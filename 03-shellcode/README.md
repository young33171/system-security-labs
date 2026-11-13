# x86 Linux Shellcode Labs

NASM으로 32-bit x86 어셈블리 코드를 작성하고, 생성된 기계어를 C 프로그램에서 실행하는 실습입니다.

## 실습 목록

| No. | Program | Assembly | C |
|---|---|---|---|
| 01 | `/usr/bin/who` | [who.s](01-who/who.s) | [who.c](01-who/who.c) |
| 02 | `/usr/bin/whoami` | [whoami.s](02-whoami/whoami.s) | [whoami.c](02-whoami/whoami.c) |
| 03 | `/bin/cat /etc/passwd` | [passwd.s](03-cat-passwd/passwd.s) | [passwd.c](03-cat-passwd/passwd.c) |
| 04 | `/bin/cat /etc/hosts` | [hosts.s](04-cat-hosts/hosts.s) | [hosts.c](04-cat-hosts/hosts.c) |
| 05 | `/bin/ps` | [ps.s](05-ps/ps.s) | [ps.c](05-ps/ps.c) |
| 06 | `/bin/ps -aef` | [ps_aef.s](06-ps-aef/ps_aef.s) | — |
| 07 | `link`, `chmod` | [학습 주제](07-link-chmod/README.md) | — |

## execve 시스템 콜

Linux i386에서 `execve`는 다음 레지스터를 사용합니다.

| Register | Value |
|---|---|
| EAX | 11 (`0x0b`) |
| EBX | 실행 파일 경로 |
| ECX | `argv` 배열 |
| EDX | `envp` 배열 |

`int 0x80`으로 시스템 콜을 실행합니다.

## 실습 절차

```bash
nasm -f bin who.s -o who
ndisasm -b32 who > dump_w
perl /root/convert.pl dump_w out_w
gcc -m32 -z execstack -o whoS who.c
./whoS
```

## 구현 포인트

- 명령어 경로와 인자를 메모리에 배치
- NUL 종료 문자열 구성
- `argv` 배열에 경로 및 인자 포인터 저장
- C에서 `\xNN` 형식의 바이트 문자열 사용
- 실행 메모리 권한과 프로그램 동작 확인
