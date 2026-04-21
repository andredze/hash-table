default rel

global CountHashCrc32Asm

section .text

;==================================================================
;------------------------------------------------------------------
; uint32_t CountHashCrc32(const char string[64])
;------------------------------------------------------------------
; Short:   Crc32 hash function implementation
; In:      rdi --> string[64]
; Out:     rax = hash value
; Destroy: -
;------------------------------------------------------------------
; C-equivalent using intrinsics
;------------------------------------------------------------------
; uint32_t CountHashCrc32Intrinsic(char* string)
; {
;     uint32_t crc = 0xFFFFFFFF;
;
;     int length = NODE_STR_SIZE;
;
;     while (length > 0)
;     {
;         crc = _mm_crc32_u64(crc, *(uint64_t*) string);
;       
;         string += 8;
;         length -= 8;
;     }
;
;     // no remaining bytes as NODE_STR_SIZE % 8 == 0
;
;     return crc ^ 0xFFFFFFFF;
; }
;------------------------------------------------------------------

CountHashCrc32AsmOld:
    push rbp
    mov rbp, rsp
    
    ; eax = current hash value
    ; initial is all 1s equivalent to -1
    mov eax, -1
    ; loop counter
    mov rcx, STRING_SIZE_IN_CHUNKS

.NextChunkHash:
    ; accumulate a crc value with current chunk of data (8 bytes of the string)
    crc32 rax, qword [rdi]
    ; go to next chunk of data (string += 8)
    add rdi, 8

    loop .NextChunkHash

    ; crc ^ 0xFF..FF (32 ones)
    mov ecx, -1
    xor eax, ecx

    pop rbp

    ret

;==================================================================

CountHashCrc32Asm:
    push rbp
    mov rbp, rsp
    
    ; eax = current hash value
    ; initial is all 1s equivalent to -1
    mov eax, -1
    
    ; accumulate a crc value with current chunk of data (8 bytes of the string)
    crc32 rax, qword [rdi+ 0]
    crc32 rax, qword [rdi+ 8]
    crc32 rax, qword [rdi+16]
    crc32 rax, qword [rdi+24]
    crc32 rax, qword [rdi+32]
    crc32 rax, qword [rdi+40]
    crc32 rax, qword [rdi+48]
    crc32 rax, qword [rdi+56]

    ; crc ^ 0xFF..FF (32 ones)
    mov ecx, -1
    xor eax, ecx

    pop rbp

    ret

;==================================================================

section .rodata

;==================================================================

; Size of string (argument for a hash function) in bytes
STRING_SIZE equ 64

; Size of string (argument for a hash function) in chunks of 8 bytes
STRING_SIZE_IN_CHUNKS equ (STRING_SIZE / 8)

;==================================================================

section .note.GNU-stack
