BITS 32
section .text
global _start
_start:
    jmp short data
run:
    pop esi
    xor eax,eax
    mov byte [esi+7],al
    mov byte [esi+12],al
    lea ebx,[esi]
    lea edi,[esi+8]
    push eax
    push edi
    push ebx
    mov ecx,esp
    xor edx,edx
    mov al,11
    int 0x80
data:
    call run
    db '/bin/ps',0x41
    db '-aef',0x41
