# System Security Labs

**Linux System Security · x86 Assembly · Shellcode · Memory Safety · System Calls**

Kali Linux 가상머신에서 진행한 시스템보안 실습을 주제별로 정리한 저장소입니다. 운영체제의 프로세스 실행 구조와 메모리 보호 원리를 학습하고, x86 어셈블리로 작성한 프로그램을 기계어로 변환해 C 환경에서 실행하는 과정을 다뤘습니다. 명령어를 실행하는 데 그치지 않고, 각 단계에서 레지스터와 메모리가 어떻게 변하는지, 오류가 발생하는 이유가 무엇인지 분석하는 데 중점을 두었습니다.

## 1. Development Environment

| Category | Environment |
|---|---|
| Operating System | Kali Linux (32-bit, i386) |
| Virtualization | Oracle VirtualBox |
| Languages | C, NASM x86 Assembly, Perl |
| Compiler | GCC (`-m32`) |
| Assembler | NASM (`-f bin`) |
| Disassembler | NDISASM (`-b32`) |
| Editor | Geany, Linux terminal |
| Shared directory | `/media/sf_SystemSecurity` |
| Main topics | Linux permissions, SetUID, stack frame, buffer overflow, `execve`, shellcode |

## 2. Repository Structure

```text
system-security-labs/
├── 01-linux-basics/
│   └── README.md
├── 02-stack-buffer-overflow/
│   └── README.md
├── 03-shellcode/
│   ├── README.md
│   ├── 01-who/          # who.s, who.c
│   ├── 02-whoami/       # whoami.s, whoami.c
│   ├── 03-cat-passwd/   # passwd.s, passwd.c
│   ├── 04-cat-hosts/    # hosts.s, hosts.c
│   ├── 05-ps/          # ps.s, ps.c
│   ├── 06-ps-aef/      # ps_aef.s
│   └── 07-link-chmod/  # file system syscall notes
└── docs/
    ├── workflow.md
    └── troubleshooting.md
```

## 3. Lab Overview

| Lab | Topic | Main focus |
|---|---|---|
| 01 | Linux File Permissions | rwx 비트, 8진수 권한, 파일 소유자/그룹 |
| 02 | SetUID and Process Credentials | Real UID / Effective UID, 특수 권한의 보안 의미 |
| 03 | Stack Memory | 함수 호출, 지역 변수, EBP/ESP, 반환 주소 |
| 04 | Buffer Overflow | 버퍼 경계, 인접 메모리 변조, 컴파일러 보호 기법 |
| 05 | `who` Shellcode | 프로그램 경로를 메모리에 구성해 `execve` 호출 |
| 06 | `whoami` Shellcode | 프로세스 실행 사용자 확인 |
| 07 | `cat /etc/passwd` | 여러 인자를 포함하는 `argv` 구성 |
| 08 | `cat /etc/hosts` | 경로 문자열 길이와 종료 바이트 처리 |
| 09 | `ps` | 프로세스 목록 조회 |
| 10 | `ps -aef` | 옵션 문자열을 포함한 인자 배열 구성 |
| 11 | `link`, `chmod` | 파일 시스템 시스템 콜과 권한 변경 원리 |

## 4. Core Concepts

### 4.1 Linux Process Execution

프로그램을 실행하면 운영체제는 실행 파일의 코드, 데이터, 스택 및 힙을 프로세스의 가상 주소 공간에 배치합니다. 사용자 공간의 C 프로그램은 시스템 콜을 통해 커널에 파일 접근, 프로세스 생성 및 프로그램 교체 등을 요청합니다.

32-bit Linux의 전통적인 `int 0x80` 인터페이스에서 `execve`는 시스템 콜 번호 **11**을 사용합니다.

| Register | Purpose |
|---|---|
| `EAX` | 시스템 콜 번호 (`11`) |
| `EBX` | 실행할 파일 경로 문자열 포인터 |
| `ECX` | `argv` 포인터 배열 |
| `EDX` | `envp` 포인터 배열 |

`execve(path, argv, envp)`가 성공하면 현재 프로세스의 프로그램 이미지가 새로운 실행 파일로 교체됩니다. 따라서 일반적으로 성공 후에는 기존 호출 코드로 복귀하지 않습니다. 실패할 때만 음수 오류 코드가 반환됩니다.

### 4.2 From Assembly to Machine Code

```text
NASM source (.s)
    │ nasm -f bin
    ▼
Raw binary
    │ ndisasm -b32
    ▼
Instruction bytes + disassembly
    │ convert.pl / manual byte inspection
    ▼
C byte string (\xNN)
    │ GCC -m32
    ▼
Executable test harness
```

이 과정에서 어셈블리 명령어의 길이, 상대 점프 오프셋, 문자열의 실제 바이트 수, 널 종료 위치가 중요합니다. 특히 `jmp-call-pop`은 코드 안에 배치한 문자열의 주소를 실행 시점에 얻는 기법입니다.

### 4.3 Stack and Buffer Overflow

스택 프레임은 함수 실행에 필요한 지역 변수, 저장된 프레임 포인터, 반환 주소 등을 담습니다. 버퍼 크기를 초과한 쓰기는 인접 메모리를 손상시킬 수 있으므로, 입력 길이 검사와 메모리 안전성 확보가 필수적입니다. 관련 방어 기법으로 **Stack Canary, NX, ASLR, PIE**를 함께 학습했습니다.

## 5. Shellcode Exercises

### Exercise 01 — who

**Objective:** `/usr/bin/who`를 실행하여 로그인 세션 정보를 확인합니다.

- 경로 문자열을 메모리에 배치
- `execve`의 경로 및 인자 포인터 구성
- 실행 결과에서 세션/터미널 정보 확인

[Assembly](03-shellcode/01-who/who.s) · [C](03-shellcode/01-who/who.c)

### Exercise 02 — whoami

**Objective:** `/usr/bin/whoami`로 현재 프로세스의 실행 사용자 이름을 출력합니다.

- 문자열 종료 바이트를 런타임에 구성
- 바이트 문자열의 `\xNN` 표기와 실제 기계어 길이 확인
- 실습 환경에서 `root` 출력 확인

[Assembly](03-shellcode/02-whoami/whoami.s) · [C](03-shellcode/02-whoami/whoami.c)

### Exercise 03 — cat /etc/passwd

**Objective:** `/bin/cat`에 파일 경로 인자를 전달합니다.

```c
argv[0] = "/bin/cat";
argv[1] = "/etc/passwd";
argv[2] = NULL;
```

- 하나의 문자열이 아닌 두 개의 독립된 인자 문자열 사용
- `argv` 배열의 포인터 순서 확인
- `/etc/passwd`의 계정 항목 출력

[Assembly](03-shellcode/03-cat-passwd/passwd.s) · [C](03-shellcode/03-cat-passwd/passwd.c)

### Exercise 04 — cat /etc/hosts

**Objective:** `/etc/hosts` 파일을 출력합니다.

- 앞선 `cat` 예제를 기반으로 경로 변경
- 문자열 길이에 따른 NUL 바이트 위치 조정
- `localhost`와 Kali 호스트 매핑 확인

[Assembly](03-shellcode/04-cat-hosts/hosts.s) · [C](03-shellcode/04-cat-hosts/hosts.c)

### Exercise 05 — ps

**Objective:** `/bin/ps`를 실행해 현재 프로세스 목록을 조회합니다.

- 단일 프로그램 경로와 `argv` 배열 구성
- `PID`, `TTY`, `TIME`, `CMD` 필드 이해

[Assembly](03-shellcode/05-ps/ps.s) · [C](03-shellcode/05-ps/ps.c)

### Exercise 06 — ps -aef

**Objective:** `/bin/ps`에 `-aef` 옵션을 전달합니다.

```c
argv[0] = "/bin/ps";
argv[1] = "-aef";
argv[2] = NULL;
```

- 두 문자열의 시작 주소와 종료 바이트 계산
- 옵션 인자의 순서와 프로세스 조회 범위 이해

[Assembly](03-shellcode/06-ps-aef/ps_aef.s)

### Exercise 07 — link and chmod

하드 링크와 파일 권한 변경을 통해 파일 시스템 시스템 콜의 역할을 학습합니다.

[Notes](03-shellcode/07-link-chmod/README.md)

## 6. Build & Run

```bash
# 1) 어셈블리 소스에서 raw binary 생성
nasm -f bin who.s -o who

# 2) 32-bit 명령어로 역어셈블
ndisasm -b32 who > dump_w

# 3) 수업에서 사용한 변환 스크립트로 C 바이트 문자열 생성
perl /root/convert.pl dump_w out_w

# 4) 변환 결과를 C 코드의 shellcode 배열에 반영
# 5) 32-bit C 프로그램 빌드
gcc -m32 -z execstack -o whoS who.c

# 6) 실행
./whoS
```

수업 환경의 C 래퍼는 `mmap`으로 실행 가능한 메모리를 할당하고 쉘코드 바이트를 복사해 호출합니다. 이러한 RWX 메모리 설정은 교육용 가상머신에서만 사용합니다.

## 7. Troubleshooting Highlights

| Issue | Analysis | Resolution |
|---|---|---|
| `No such file or directory` | 현재 디렉터리와 소스 파일 위치가 다름 | `pwd`, `ls`, 절대경로로 확인 |
| `unknown escape sequence '\X'` | 잘못된 C 문자열 이스케이프 | 소문자 `\xNN` 사용 |
| `Segmentation fault` | 잘못된 바이트 또는 메모리 권한 문제 | 바이트 배열과 실행 권한 확인 |
| `-z execstack ignored` | 링커 옵션이 적용되지 않음 | 경고 원인과 실제 실행 경로를 분리해 확인 |
| 출력 크기 불일치 | NUL 또는 문자열 길이 계산 오류 | `sizeof(shellcode)-1`과 바이트 배열 비교 |
| 잘못된 실행 경로 | 문자열 조립 시 슬래시/문자 순서 오류 | 기계어 바이트와 원래 경로 대조 |

[Detailed troubleshooting](docs/troubleshooting.md)

## 8. 실습 결과 및 학습 성과

실습별 실행 내용, 관찰한 출력과 학습한 기술을 [Lab Results & Learning Outcomes](docs/lab-results.md)에 정리했습니다. 각 실습 폴더의 README에는 NASM/C 소스 전문과 구현 분석을 함께 수록했습니다.

## 9. Key Takeaways

- Linux 사용자 공간과 커널 공간이 시스템 콜로 연결되는 구조를 이해했습니다.
- NASM 명령어를 기계어로 변환하고 각 바이트의 의미를 추적했습니다.
- 단일 명령 실행과 여러 인자를 받는 프로그램 실행의 차이를 학습했습니다.
- 프로그램 경로, 포인터 배열, NUL 종료 처리의 중요성을 확인했습니다.
- 컴파일 성공만으로 프로그램의 정상 동작을 판단할 수 없다는 점을 경험했습니다.
- 스택 메모리와 파일 권한이 시스템보안에서 갖는 의미를 학습했습니다.

---

**Scope:** 개인 Kali Linux 가상머신 기반 시스템보안 수업 실습. 교육 및 학습 목적으로 정리했습니다.


## AI and Data Analysis Coursework

A separate study area documents the Python 3.11/uv environment and PyTorch Tensor exercises for Part 1–5.

- [AI and data analysis index](04-ai-data-analysis/README.md)
- [PyTorch Tensor lab guide and sanitized notebook](04-ai-data-analysis/01-pytorch-tensor-basics/README.md)
- [Final submitted files](04-ai-data-analysis/submissions/README.md) — exact report and notebook files are included at the user's request.
