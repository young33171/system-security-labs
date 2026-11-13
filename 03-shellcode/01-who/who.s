BITS 32
section .text
global _start
_start:
    xor eax,eax
    push eax
    push 0x6f68772f
    push 0x6e69622f
    push 0x7273752f
    mov ebx,esp
    push eax
    mov edx,esp
    push ebx
    mov ecx,esp
    mov al,11
    int 0x80
