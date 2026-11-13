# 32-bit x86 Linux Shellcode 실습

Linux i386 `execve` 호출 관례를 바탕으로 NASM 코드를 기계어로 만들고, `ndisasm` 및 교수님 제공 `convert.pl`을 거쳐 C 문자열로 변환하는 실습입니다.

| 과제 | 경로 | 상태 |
|---|---|---|
| 01 | [who](01-who/) | 출력 확인 |
| 02 | [whoami](02-whoami/) | 출력 확인 |
| 03 | [cat passwd](03-cat-passwd/) | 출력 확인 |
| 04 | [cat hosts](04-cat-hosts/) | 출력 확인 |
| 05 | [ps](05-ps/) | 변환 확인, 실행 미확인 |
| 06 | [ps -aef](06-ps-aef/) | 복원 예제, 실행 미확인 |
| 07 | [link / chmod](07-link-chmod/) | 과제만 확인 |

## 핵심 원리
- NASM `BITS 32` / `global _start`
- i386: eax=11(0x0b) 지정 후 `int 0x80`으로 `execve` 호출
- ebx=실행할 파일 경로, ecx=argv 배열, edx=envp 배열
- 리틀 엔디언 상수와 문자열의 NUL 종료 처리
- `jmp/call/pop`으로 데이터 주소 획득, C 배열의 `\xNN` 바이트 표현
- `execve` 성공 시 호출 프로세스가 프로그램 이미지로 교체됨

`convert.pl`, `dump_*`, `out_*`는 실제 VM 파일을 별도로 가져와 검증 후 추가해야 합니다. 첨부된 소스는 수업 실습 중 채팅에 나온 내용을 기준으로 재구성한 버전입니다.
