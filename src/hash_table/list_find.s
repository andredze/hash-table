default rel

global ListFindElemAsm

section .text

ListFindElemAsm:
    mov        r9,QWORD PTR [rdi]
    mov        rdi,QWORD PTR [rdi+0x10]
    mov        r8d,DWORD PTR [r9+0x8]
    test       rdi,rdi
    je         6f
    vmovdqa64  zmm1,ZMMWORD PTR [rsi]
    xor        ecx,ecx
    jmp        2c
    xchg       ax,ax
    inc        rcx
    mov        r8d,DWORD PTR [rax+0x8]
    cmp        rdi,rcx
    je         60
    movsxd     rax,r8d
    shl        rax,0x4
    add        rax,r9
    mov        rsi,QWORD PTR [rax]
    vpcmpneqw  k0,ZMMWORD PTR [rsi],zmm1             
    kortestd   k0,k0
    jne        20
    vzeroupper
    mov        DWORD PTR [rdx],r8d
    xor        eax,eax
    ret
    cs         nop WORD PTR [rax+rax*1+0x0]
    mov        r8d,0xffffffff
    vzeroupper
    xor        eax,eax
    mov        DWORD PTR [rdx],r8d
    ret
    mov        r8d,0xffffffff
    jmp        50

section .note.GNU-stack
