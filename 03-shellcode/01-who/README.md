# Lab 01 — who

## 1. 실습 목표

현재 시스템에 로그인한 사용자와 터미널 세션을 조회하는 명령을 Linux i386 execve 시스템 콜로 실행한다.

- **실행 대상:** `/usr/bin/who`
- **개발 환경:** Kali Linux i386 / NASM / NDISASM / GCC / Geany
- **작업 위치:** `/media/sf_SystemSecurity`
- **진행 내용:** 실습 당시 로그인 세션 목록 출력 확인

## 2. 사전 개념

32-bit Linux의 `execve` 시스템 콜 번호는 11이다. `int 0x80`으로 커널에 진입할 때 다음 레지스터에 인자를 배치한다.

| Register | Value |
|---|---|
| EAX | 11 (execve) |
| EBX | 실행 파일 경로의 주소 |
| ECX | NULL로 끝나는 argv 포인터 배열 |
| EDX | 환경 변수 포인터 배열 또는 NULL |

C 언어 관점의 인자 배열은 다음과 같다.

```c
argv[0] = "/usr/bin/who"; argv[1] = NULL;
```

`execve` 호출에 성공하면 기존 프로세스 이미지가 새 프로그램으로 교체된다. 실행에 실패했을 때만 기존 호출 흐름으로 돌아온다.

## 3. 소스 코드 구성 및 분석

- [who.s](who.s) — NASM 어셈블리 소스
- [who.c](who.c) — 쉘코드 바이트를 실행하는 C 프로그램

`push`로 경로 문자열을 역순으로 스택에 배치한다. x86의 리틀 엔디언 특성상 `push 0x6f68772f`는 메모리에 `/who` 순서로 놓인다. 같은 방식으로 `/bin`, `/usr`를 결합하고 `EBX`에 스택 주소를 저장한다.

### 어셈블리에서 확인할 명령어

- `xor eax, eax`: EAX를 0으로 초기화한다.
- `mov al, 11`: 시스템 콜 번호를 설정한다.
- `push`: 스택에 값 또는 포인터를 배치한다.
- `mov ecx, esp`: 스택에 구성한 argv 배열의 주소를 설정한다.
- `int 0x80`: Linux i386 시스템 콜을 호출한다.

경로를 4바이트 단위로 스택에 넣는 방식은 리틀 엔디언 저장 순서를 이해하는 데 유용하다.


## 4. 빌드 및 실행 절차

```bash
cd /media/sf_SystemSecurity
nasm -f bin who.s -o who
ndisasm -b32 who > dump_who
```

변환 스크립트 `/root/convert.pl`은 역어셈블 결과를 C에서 사용하는 바이트 문자열로 변환하는 데 사용한다. 원본 파일 이름과 실제 변환 결과를 확인한 뒤 C 배열에 반영한다.

```bash
gcc -m32 -z execstack -o whoS who.c
./whoS
```

### 바이트 길이

현재 저장소의 소스 구성 기준 기계어 길이는 **30 bytes**이다. 길이는 NASM 바이너리로 확인해야 하며 C 문자열 끝의 자동 NUL 바이트는 제외한다.

## 5. 실행 결과 및 해석

**진행 기록:** 터미널에 로그인 사용자와 `tty`/`pts` 세션 정보가 출력되는 것을 확인했다.

**출력 해석:** 사용자 이름, 터미널 이름, 접속 시각 등이 행 단위로 표시된다.

실제 출력은 사용자 계정, 로그인 세션, 실행 중인 프로세스 등 가상머신 상태에 따라 달라진다. 위 내용은 실행 당시 관찰한 항목과 명령어의 일반적인 출력 형식을 구분해 기록했다.

## 6. 문제 해결과 점검 포인트

1. **파일 경로:** `pwd`, `ls`로 어셈블리와 C 파일이 존재하는 폴더를 확인한다.
2. **바이트 문자열:** C 코드에서는 `\\xNN` 형태를 사용하며, 대문자 `\\X`를 사용하지 않는다.
3. **문자열 종료:** 경로와 옵션 인자가 각각 NULL로 끝나는지 확인한다.
4. **argv 포인터:** 첫 번째 요소는 프로그램 경로, 이후 요소는 옵션/파일명이며 마지막은 NULL이다.
5. **실행 권한:** 실습용 C 래퍼의 `mmap` 메모리 권한과 함수 포인터 호출을 확인한다.
6. **실제 동작:** 컴파일이 성공했다고 해서 명령 실행까지 성공한 것은 아니므로 프로그램 출력을 확인한다.

## 7. 학습 성과

리틀 엔디언에 따른 메모리 저장 순서, 스택에 문자열을 배치하는 방법, `execve`에 실행 경로와 `argv`를 전달하는 구조.

이번 실습에서 가장 중요한 부분은 실행할 명령의 문자열을 바꾸는 것보다 **문자열이 메모리에 놓이는 위치와 이를 참조하는 포인터 배열이 올바른지**를 확인하는 과정이었다. 어셈블리 소스, 생성된 바이너리, C 바이트 문자열을 연결해 이해하면서 시스템 콜의 동작을 더 구체적으로 학습했다.

## 8. 다음 학습 방향

- NASM 소스와 C 배열의 바이트 일치 여부 자동 검사
- `strace`로 `execve` 호출 인자와 반환값 관찰
- 실행 메모리 권한과 NX 정책의 관계 정리
- 오류가 발생하는 입력 조건과 정상 실행 조건 비교

> 개인 Kali Linux 가상머신에서 수행한 교육용 실습 기록이다.

## 9. 실습 소스 코드

아래 코드는 저장소의 개별 소스 파일과 동일한 내용이다. 어셈블리와 C 래퍼를 함께 읽으면서 레지스터 설정부터 실행 메모리 호출까지의 흐름을 확인할 수 있다.

### who.s

```asm
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
```

### who.c

```c
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

/* 교육용 Kali VM 전용: RWX 메모리를 실서비스에 사용하지 마세요. */
unsigned char shellcode[] = 
"\x31\xc0\x50\x68\x2f\x77\x68\x6f\x68\x2f\x62\x69\x6e\x68\x2f\x75"
"\x73\x72\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

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
```

