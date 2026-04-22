default rel

global HashTableFindElementAsm

section .text

extern CountHashCrc32Asm

;==================================================================
;------------------------------------------------------------------
; extern "C"
; HashTableErr_t HashTableFindElementAsm(HashTable_t*    hash_table,
;                                        HashTableElem_t item,
;                                        int*            hash_table_pos,
;                                        int*            list_pos);
;------------------------------------------------------------------
; Short:   Hash table find element function implementation
; In:      rdi --> hash_table
;          rsi --> item
;          rdx --> hash_table_pos (dest)
;          rcx --> list_pos       (dest)
; Out:     rax = error status
; Destroy: -
;------------------------------------------------------------------
; C-equivalent
;------------------------------------------------------------------
; HashTableErr_t HashTableFindElement(HashTable_t*    hash_table,
;                                     HashTableElem_t item,
;                                     int*            hash_table_pos,
;                                     int*            list_pos)
; {
;     if (hash_table->capacity == 0)
;     {
;         if (hash_table_pos)
;         {
;             *hash_table_pos = -1;
;         }
;         if (list_pos)
;         {
;             *list_pos = -1;
;         }

;         return HT_SUCCESS;
;     }
;
;     size_t hash_value = CountHashCrc32Asm(item);
;     size_t elem_index = hash_value % hash_table->capacity;
;     List_t* list_ptr = &hash_table->data[elem_index];
;
;     int pos = -1;
;
;     if (ListFindElement(list_ptr, item, &pos) != LIST_SUCCESS)
;     {
;         return HT_LIST_ERROR;
;     }
;
;     // (pos == -1) if element was not found
;
;     if (hash_table_pos)
;     {    
;         *hash_table_pos = (int) elem_index;
;     }
;     if (list_pos)
;     {
;         *list_pos = pos;
;     }
;
;     return HT_SUCCESS;
; }
;------------------------------------------------------------------
; HashTable struct fields:
;------------------------------------------------------------------
; typedef struct HashTable
; {
;     size_t      capacity;
;     size_t      size;
;     List_t*     data;
;     uint32_t (* hash_function)(const HashTableElem_t elem);
;     Word_t*     words;
; }
; HashTable_t;
;------------------------------------------------------------------
; Preserve: rbx, rsp, rbp, r12, r13, r14, r15
;------------------------------------------------------------------

HashTableFindElementAsm:
    push rbp

    ; r8 = hash_table->capacity
    mov r8, [rdi]
    test r8, r8
    ; if (hash_table->capacity == 0)
    jz .ZeroCapacity

    ; r9 --> hash_table_pos (dest)
    ; save for using rdx for div
    mov r9, rdx

    ; rbp --> hash_table
    mov rbp, rdi

    ; rdi --> item
    mov rdi, rsi

    call CountHashCrc32Asm
    ; rax = hash value
    ; set rdx to 0 for div
    xor edx, edx
    ; divide hash / capacity
    div r8
    ; rdx = hash_table_pos = elem_index
    ; save rdx in r10
    mov r10, rdx

    ; hash_table->data + 32 * rdx
    shl rdx, 5
    add rdx, QWORD [rbp + 0x10]
    ; rdi --> hash_table->data[elem_index]
    mov rdi, rdx

    ; preserve r9
    mov rbp, r9
    ; preserve rcx
    mov rdx, rcx
    call ListFindElementAsm
    mov rcx, rdx
    mov r9, rbp

.PutTableAndListPos:
    ; check for NULL ptr
    test r9, r9
    jz .SkipHashTablePos
    ; if not NULL:
    ; *hash_table_pos = r10
    mov DWORD [r9], r10d
    .SkipHashTablePos:

    ; check for NULL ptr
    test rcx, rcx
    jz .SkipListPos

    ; if not NULL:
    ; *list_pos = r11
    mov DWORD [rcx], r11d
    .SkipListPos:

    ; eax = HT_SUCCESS
    xor eax, eax

    pop rbp

    ret

.ZeroCapacity:
    ; hash_table_pos = -1
    mov r10, -1
    ; list_pos = -1
    mov r11, -1

    jmp .PutTableAndListPos

;==================================================================
;------------------------------------------------------------------
; ListErr_t ListFindElementAsm(List_t* list, elem_t item, int* item_pos)
;------------------------------------------------------------------

ListFindElementAsm:
    ; r8 = *list = data
    mov r8, QWORD [rdi]
    ; r9 = list->size
    mov r9, QWORD [rdi+0x10]
    ; rax --> item
    mov rax,rsi
    
    ; zmm0 = *item = mm_item
    vmovdqa64 zmm0, [rax]
    ; esi = list->data[0].next
    mov esi, DWORD [r8+0x8]

    ; if r9 == 0
    test r9,r9
    je .NotFound
    ; ecx = i = 0
    xor ecx, ecx
    jmp .Compare
    
    nop

.LoopNext:
    ; i++
    inc rcx
    mov esi, DWORD [rax+0x8]
    ; if i == list_size
    cmp r9, rcx
    je .NotFound

.Compare:
    ; rax --> data + node_pos * 16 == next_node_ptr
    ; rax = esi = next_pos
    movsxd rax, esi
    ; node_pos * 16
    shl rax, 0x4
    add rax,r8
    
    ; rdi = *next_node = char* --> data
    mov rdi, QWORD [rax]
    
    ; cmpne mm_item with *next_data and store mask in k0
    vpcmpneqw k0, zmm0, [rdi]
    ; if zero -> set zf (if equal ==> zf = 1)
    kortestd k0, k0
    ; if not found --> next
    jne .LoopNext
    
    ; if equal, set *item_pos = node_pos
    mov r11d, esi
    
    ; zero ymms zmms higher bits
    vzeroupper
    ret

.NotFound:
    ; *item_pos = -1
    mov r11d, 0xffffffff
    
    ; zero ymms and zmms higher bits
    vzeroupper
    ret

;==================================================================

section .note.GNU-stack
