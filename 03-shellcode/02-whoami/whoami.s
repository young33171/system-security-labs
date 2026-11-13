BITS 32
section .text
global _start
_start:
    jmp short path
run:
    pop ebx
    xor eax,eax
    mov byte [ebx+15],al
    push eax
    mov edx,esp
    push ebx
    mov ecx,esp
    mov al,11
    int 0x80
path:
    call run
    db '/usr/bin/whoami',0x41
