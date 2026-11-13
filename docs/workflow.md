# Shellcode Lab Workflow

## 1. 실습 환경 확인

실습에 앞서 현재 위치와 컴파일러, 어셈블러의 설치 여부를 확인합니다.

```bash
pwd
uname -m
which nasm
which ndisasm
which gcc
which perl
ls -l /root/convert.pl
```

`uname -m`은 커널 아키텍처를 보여줍니다. 32-bit 코드를 빌드할 때는 GCC에 `-m32` 옵션을 지정하며, 해당 환경에 32-bit 개발 라이브러리가 있어야 합니다.

## 2. NASM 소스 작성

Geany에서 `.s` 파일을 작성합니다.

```bash
geany /media/sf_SystemSecurity/who.s
```

소스 상단에는 `BITS 32`, `section .text`, `global _start` 등을 지정합니다. `BITS 32`는 32-bit x86 명령어 인코딩을 사용하도록 합니다.

## 3. Raw Binary 생성

```bash
cd /media/sf_SystemSecurity
nasm -f bin who.s -o who
ls -l who
file who
```

`-f bin`은 ELF 헤더가 없는 순수 기계어 바이트를 출력합니다. 따라서 이 파일을 일반 ELF 실행 파일처럼 직접 실행하는 것은 적절하지 않습니다.

## 4. Disassembly

```bash
ndisasm -b32 who > dump_w
cat dump_w
```

`ndisasm`은 생성된 바이트를 명령어로 다시 해석합니다. 이 단계에서 예상한 명령어가 맞는지, 상대 분기 오프셋이 정확한지 확인합니다.

## 5. C 문자열 변환

```bash
perl /root/convert.pl dump_w out_w
cat out_w
```

변환 결과를 C 소스의 `unsigned char shellcode[]`에 반영합니다. `\xNN`이 실제로 어떤 바이트인지 확인하는 것이 중요합니다. 잘못된 이스케이프는 컴파일 경고나 런타임 오류로 이어질 수 있습니다.

## 6. C 래퍼

실습용 C 래퍼의 기본 흐름은 다음과 같습니다.

1. `sizeof(shellcode)-1`로 배열 길이 계산
2. `mmap`으로 메모리 할당
3. `memcpy`로 기계어 바이트 복사
4. 함수 포인터로 해당 주소 호출

`mmap`에서 `PROT_READ | PROT_WRITE | PROT_EXEC`를 함께 사용하는 방식은 학습을 위한 것으로, 일반적인 서비스 코드에서는 권장하지 않습니다.

## 7. Compile & Run

```bash
gcc -m32 -z execstack -o whoS who.c
./whoS
```

실습 환경에서 `-z execstack ignored` 경고가 표시될 수 있습니다. 이 경고만으로 실행 성공 여부를 판단하지 않고 실제 프로그램 출력과 메모리 실행 경로를 함께 확인합니다.

## 8. 명령어별 예상 출력 형태

| Command | Typical output |
|---|---|
| `who` | 로그인 사용자, 터미널, 접속 시각 |
| `whoami` | 현재 사용자 이름 |
| `cat /etc/passwd` | 사용자 계정 항목 |
| `cat /etc/hosts` | 호스트 이름과 IP 매핑 |
| `ps` | PID, TTY, TIME, CMD 등 |
| `ps -aef` | 사용자, PID, PPID, 실행 명령 등 |

출력 값은 시스템 상태와 배포판에 따라 달라집니다.

## 9. 파일 위치 관리

가상머신 내부의 `/root`와 VirtualBox 공유 폴더 `/media/sf_SystemSecurity`를 함께 사용했습니다. 컴파일 오류가 발생하면 먼저 현재 경로를 확인합니다.

```bash
pwd
ls -lah
find /media/sf_SystemSecurity -maxdepth 1 -type f
```

## 10. 체크 포인트

- [ ] 소스 파일이 저장된 위치가 올바른가?
- [ ] NASM 컴파일 오류가 없는가?
- [ ] `ndisasm`의 명령어가 예상과 일치하는가?
- [ ] 문자열의 종료 바이트 위치가 맞는가?
- [ ] `argv` 배열이 NULL 포인터로 끝나는가?
- [ ] C 배열의 `\xNN` 바이트가 실제 바이너리와 일치하는가?
- [ ] 프로그램이 예상한 명령을 실행하는가?
