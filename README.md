# System Security Labs | 시스템보안 실습 포트폴리오

> Kali Linux · x86 Assembly · NASM · C · Linux System Calls · Stack / Buffer Overflow

정보보호 전공 시스템보안 수업의 실습 코드와 오류 해결 과정을 모은 학습 저장소입니다. VirtualBox의 **Kali Linux 2016 light (32-bit)** 실습 화면을 바탕으로 재구성하였습니다. **채팅에서 확인된 성공 결과**와 **실습 원본을 추가로 확인해야 하는 항목**을 명확히 구분합니다.

## 실습 범위 및 검증

| 주제 | 학습 내용 | 검증 상태 |
|---|---|---|
| Linux 권한/SetUID | user/group/other, UID/EUID, chmod | 과거 학습 기록, 원본 파일 대조 필요 |
| 스택/버퍼 오버플로우 | stack_example.c, buf_ret.c, 반환 주소, 보호 기법 | 학습 기록, 원본 코드·캡처 필요 |
| 01 who | /usr/bin/who | ✅ 실행 화면에서 tty/pts 접속 세션 확인 |
| 02 whoami | /usr/bin/whoami | ✅ 'root' 출력 확인 |
| 03 cat passwd | /bin/cat /etc/passwd | ✅ 계정 목록 출력 확인 |
| 04 cat hosts | /bin/cat /etc/hosts | ✅ localhost/kali 출력 확인 |
| 05 ps | /bin/ps | NASM 및 변환 완료, C 실행 캡처 미확인 |
| 06 ps -aef | /bin/ps -aef | 소스 복원, 원본 실습 및 실행 미확인 |
| 07 link/chmod | Linux link/chmod syscall | 과제 내용 확인, 구현 및 실행 미확인 |

**중요:** 이 저장소의 코드는 채팅 중 작성한 실습 예제를 근거로 정리한 복원본이며, Kali 가상머신에 저장된 파일을 자동으로 가져온 것은 아닙니다. 성공하지 않은 과제에 성공 표시를 하지 않았습니다.

## 프로젝트 구조

- [01-linux-basics](01-linux-basics/README.md) — 권한, SetUID, UID/EUID
- [02-stack-buffer-overflow](02-stack-buffer-overflow/README.md) — 스택 프레임, 버퍼 오버플로우
- [03-shellcode](03-shellcode/README.md) — 1~7번 쉘코드 과제 및 소스
- [docs/workflow.md](docs/workflow.md) — 제작·컴파일·실행 프로세스
- [docs/troubleshooting.md](docs/troubleshooting.md) — 실제 겪은 오류와 수정
- [docs/verification.md](docs/verification.md) — 검증 기록과 남은 작업
- [evidence/README.md](evidence/README.md) — 터미널 캡처 제출 지침

## 환경

- VirtualBox, Kali Linux 2016 light (i686)
- NASM x86 32-bit, ndisasm, Perl, GCC -m32
- 작업 경로: /root, /media/sf_SystemSecurity
- Linux i386 execve 시스템 콜: eax=11, int 0x80

## 쉘코드 실습 기본 명령

```bash
nasm -f bin who.s -o who
ndisasm -b32 who > dump_w
perl /root/convert.pl dump_w out_w
# 생성된 바이트를 C의 shellcode[]에 반영
gcc -m32 -z execstack -o whoS who.c
./whoS
```

**실습 환경 주의:** 위 환경에서 링커가 '-z execstack ignored' 경고를 표시했습니다. 당시 실습은 교육용으로 mmap의 실행 가능 페이지에 shellcode를 복사하여 호출하는 C 래퍼를 사용했습니다. RWX 메모리는 실서비스에 사용하지 않습니다.

## 학습 성과

1. Linux 권한, UID/EUID와 프로세스 권한 모델의 이해
2. 스택 프레임, 반환 주소, 버퍼 오버플로우와 ASLR/NX/Stack Canary의 관계 학습
3. NASM → 기계어 → 역어셈블 → C 바이트 문자열 변환
4. execve(path, argv, envp), 레지스터 인자 전달과 리틀 엔디언 문자열 배치
5. C의 '\xNN' 인코딩, 널 바이트, 메모리 실행 권한 및 Segmentation fault 디버깅

## 증빙/윤리

실습 출력 스크린샷은 추가 업로드 대상입니다. /etc/passwd 등 계정 또는 환경 정보가 포함될 수 있는 원본 결과는 공개 전에 마스킹합니다. 이 저장소는 본인이 관리하는 실습 VM에서의 교육·분석만을 목적으로 합니다.


## Other coursework

This repository also keeps non-security course labs in a separate directory so they do not mix with the system-security exercises.

- [AI and data analysis](04-ai-data-analysis/README.md) — [PyTorch Tensor basics](04-ai-data-analysis/01-pytorch-tensor-basics/README.md), including the Python/uv environment notes, Part 2–5 explanations, requirements, and an executed notebook.

The public notebook copy excludes the submitter's name and student number. The submitted report PDF and personal screenshots are kept out of this public repository.
