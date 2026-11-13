# System Security Labs

Kali Linux 환경에서 진행한 시스템보안 실습을 정리한 저장소입니다. Linux 권한 관리와 스택 메모리 구조를 학습하고, NASM을 이용해 x86 어셈블리 코드를 작성한 뒤 C 프로그램에서 실행하는 과정을 다뤘습니다.

## Tech Stack

- **OS:** Kali Linux (32-bit), VirtualBox
- **Languages:** x86 Assembly (NASM), C, Perl
- **Tools:** GCC, NASM, NDISASM, Geany, Linux CLI
- **Topics:** Linux System Calls, execve, Process, Stack Memory, Buffer Overflow, SetUID

## Contents

| Directory | Description |
|---|---|
| [01-linux-basics](01-linux-basics/) | Linux 파일 권한, UID/EUID, SetUID |
| [02-stack-buffer-overflow](02-stack-buffer-overflow/) | 스택 구조와 버퍼 오버플로우 |
| [03-shellcode](03-shellcode/) | x86 Linux 쉘코드 제작 실습 |
| [docs/workflow.md](docs/workflow.md) | 실습 절차와 주요 명령어 |
| [docs/troubleshooting.md](docs/troubleshooting.md) | 실습 중 발생한 오류와 해결 과정 |

## Shellcode Labs

| No. | Task | Files |
|---|---|---|
| 01 | `/usr/bin/who` | `who.s`, `who.c` |
| 02 | `/usr/bin/whoami` | `whoami.s`, `whoami.c` |
| 03 | `/bin/cat /etc/passwd` | `passwd.s`, `passwd.c` |
| 04 | `/bin/cat /etc/hosts` | `hosts.s`, `hosts.c` |
| 05 | `/bin/ps` | `ps.s`, `ps.c` |
| 06 | `/bin/ps -aef` | `ps_aef.s` |
| 07 | `link`, `chmod` system calls | 실습 항목 |

### Workflow

```bash
nasm -f bin who.s -o who
ndisasm -b32 who > dump_w
perl /root/convert.pl dump_w out_w
gcc -m32 -z execstack -o whoS who.c
./whoS
```

어셈블리 작성 → 바이너리 생성 → 역어셈블 → 쉘코드 변환 → C 프로그램 실행 순서로 진행했습니다.

## What I Learned

### Linux System Calls
32-bit Linux에서 `int 0x80` 인터럽트를 사용해 시스템 콜을 호출하는 방법과 `execve`의 인자 전달 구조를 학습했습니다.

### Assembly & Shellcode
레지스터 활용, 리틀 엔디언, 문자열 종료 처리, `jmp-call-pop` 기법을 익혔습니다. NASM 출력과 C 바이트 배열을 비교하면서 인코딩 문제를 해결했습니다.

### Debugging
컴파일 경고, 경로 오류, 잘못된 이스케이프 시퀀스, Segmentation fault를 분석하고 수정했습니다.

### Memory Security
스택 프레임과 버퍼 오버플로우의 동작 원리를 살펴보고 NX, ASLR, Stack Canary 등의 방어 기법을 공부했습니다.

## Notes

실습은 개인 Kali Linux 가상머신에서 진행했습니다. 일부 실습 코드는 수업에서 작성한 예제를 정리한 것으로, 운영 환경에 그대로 적용하기보다는 교육용으로 참고하는 것을 권장합니다.
