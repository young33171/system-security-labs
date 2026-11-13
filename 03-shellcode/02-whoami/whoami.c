#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

/* 교육용 Kali VM 전용: RWX 메모리를 실서비스에 사용하지 마세요. */
unsigned char shellcode[] = 
"\xeb\x10\x5b\x31\xc0\x88\x43\x0f\x50\x89\xe2\x53\x89\xe1\xb0\x0b"
"\xcd\x80\xe8\xeb\xff\xff\xff\x2f\x75\x73\x72\x2f\x62\x69\x6e\x2f"
"\x77\x68\x6f\x61\x6d\x69\x41";

int main(void) {
    size_t len = sizeof(shellcode) - 1;
    void *mem = mmap(NULL, len, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) { perror("mmap"); return 1; }
    memcpy(mem, shellcode, len);
    printf("Shellcode size: %zu bytes\n", len);
    fflush(stdout);
    ((void (*)(void))mem)();
    perror("execve failed");
    return 1;
}
