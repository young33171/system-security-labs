# x86 Linux Shellcode Programming

## 1. 실습 개요

32-bit Linux 환경에서 시스템 콜을 호출하는 어셈블리 프로그램을 작성하고, NASM으로 생성한 raw binary를 C 프로그램에 포함해 실행하는 실습입니다. 명령어를 단순히 호출하는 것보다 **경로 문자열과 인자 배열을 메모리에 직접 구성하는 과정**에 집중했습니다.

## 2. Linux i386 System Call Convention

전통적인 Linux i386 `int 0x80` 호출 규약은 다음과 같습니다.

| Register | Usage |
|---|---|
| EAX | 시스템 콜 번호 |
| EBX | 첫 번째 인자 |
| ECX | 두 번째 인자 |
| EDX | 세 번째 인자 |
| ESI | 네 번째 인자 |
| EDI | 다섯 번째 인자 |
| EBP | 여섯 번째 인자 |

`execve`의 시스템 콜 번호는 11입니다.

```c
int execve(const char *pathname, char *const argv[], char *const envp[]);
```

호출 시 `EBX`는 경로 문자열을, `ECX`는 NULL로 끝나는 인자 포인터 배열을, `EDX`는 환경 변수 포인터 배열을 가리킵니다.

## 3. 문자열과 포인터 구성

`/bin/cat /etc/hosts`를 실행한다고 가정하면 메모리에는 독립된 두 문자열이 필요합니다.

```text
"/bin/cat\0"
"/etc/hosts\0"

argv:
  [0] -> "/bin/cat"
  [1] -> "/etc/hosts"
  [2] -> NULL
```

문자열의 마지막 바이트가 NULL이 아니거나 포인터 배열의 순서가 틀리면 `execve`가 실패할 수 있습니다.

## 4. jmp-call-pop

프로그램 카운터와 관련된 주소를 런타임에 얻기 위한 방법입니다.

1. `jmp`로 문자열 데이터가 있는 위치까지 이동합니다.
2. `call`이 복귀 주소를 스택에 넣습니다.
3. 호출 대상에서 `pop`으로 그 주소를 꺼냅니다.
4. 꺼낸 주소를 문자열 시작 주소로 사용합니다.

이 구조는 절대 주소를 하드코딩하지 않고 코드 근처의 데이터에 접근하는 데 유용합니다. 상대 점프와 호출 명령의 오프셋은 실제 명령어 길이에 따라 달라집니다.

## 5. C Byte Strings

NASM 바이너리의 각 바이트는 C 문자열에서 `\xNN` 형식으로 표현할 수 있습니다.

```c
unsigned char example[] = "\x31\xc0\xb0\x0b";
```

주의할 점:

- `\x`는 소문자 `x`를 사용합니다.
- 16진수는 두 자리 단위로 읽는 것이 편리합니다.
- C 문자열 리터럴에는 끝에 NUL 바이트가 자동으로 붙습니다.
- `sizeof(example)-1`은 마지막 자동 NUL을 제외한 길이입니다.
- 중간에 실제 `\x00`이 들어 있으면 `strlen`으로는 전체 길이를 계산할 수 없습니다.

## 6. 실습 목록

| No. | Task | Key concept |
|---|---|---|
| 01 | [who](01-who/) | 단일 실행 경로와 프로세스 정보 |
| 02 | [whoami](02-whoami/) | 실행 사용자 확인, 바이트 길이 점검 |
| 03 | [cat passwd](03-cat-passwd/) | 파일 경로 인자를 포함한 `argv` |
| 04 | [cat hosts](04-cat-hosts/) | 문자열 길이 및 종료 바이트 |
| 05 | [ps](05-ps/) | 프로세스 조회 명령 실행 |
| 06 | [ps -aef](06-ps-aef/) | 옵션 인자 포인터 배열 |
| 07 | [link/chmod](07-link-chmod/) | 파일 시스템 시스템 콜 |

## 7. Build Pipeline

```bash
nasm -f bin who.s -o who
ndisasm -b32 who > dump_w
perl /root/convert.pl dump_w out_w
cat out_w
gcc -m32 -z execstack -o whoS who.c
./whoS
```

실습용 C 프로그램은 `mmap`으로 메모리를 할당한 뒤 `memcpy`로 바이트를 복사하고 함수 포인터로 호출합니다. 실행 가능한 쓰기 메모리(RWX)는 일반 소프트웨어 개발에서 피해야 하며 개인 실습 VM에서만 사용합니다.

## 8. 주요 오류와 원인

- **Segmentation fault:** 잘못된 기계어, 포인터 또는 메모리 접근 권한
- **실행 결과가 출력되지 않음:** `execve` 인자 배열이나 경로 구성 문제
- **쉘코드 길이 불일치:** 자동 NUL 또는 잘못된 C 이스케이프
- **경로 오류:** `/root`와 공유 폴더를 혼동
- **컴파일 경고:** 실행 스택 설정 옵션이 환경에서 무시될 수 있음

자세한 내용은 [Troubleshooting](../docs/troubleshooting.md)을 참고합니다.

## 9. 학습 결과

어셈블리 수준의 레지스터와 메모리 조작이 Linux 시스템 콜로 연결되는 과정을 이해했습니다. 특히 `execve`의 인자 포인터 구성, 문자열 종료 처리, 상대 주소 계산, C 바이트 배열 변환 및 실행 환경 설정을 반복적으로 연습했습니다.
