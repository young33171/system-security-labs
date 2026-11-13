# Troubleshooting Notes

시스템보안 실습에서 발생한 오류를 컴파일 단계, 바이너리 변환 단계, C 실행 단계로 나누어 정리했습니다.

## 1. File Not Found

### Symptom

```text
cc1: fatal error: who.c: No such file or directory
```

### Cause

파일을 저장한 위치와 현재 작업 디렉터리가 일치하지 않는 경우입니다. 실습 중 `/root`와 `/media/sf_SystemSecurity`를 혼용했습니다.

### Fix

```bash
pwd
ls -l
ls -l /media/sf_SystemSecurity
cd /media/sf_SystemSecurity
gcc -m32 -z execstack -o whoS who.c
```

파일이 있는 위치를 확인한 뒤 해당 디렉터리에서 컴파일합니다.

## 2. Command Typo

### Symptom

```text
readlf: command not found
```

### Cause

명령어 `readelf`를 `readlf`로 잘못 입력했습니다.

### Fix

```bash
readelf -h whoS
readelf -W -l whoS
```

`readelf`는 ELF 실행 파일의 헤더와 프로그램 헤더를 분석하는 도구입니다. NASM의 `-f bin`으로 만든 raw binary는 ELF 파일이 아니므로 C로 빌드한 실행 파일을 대상으로 사용합니다.

## 3. NASM Immediate Operand Size

### Symptom

```text
dword data exceeds bounds
```

### Cause

32-bit 명령어에 허용 범위를 초과하는 즉시값을 넣은 경우입니다. 여러 글자의 문자열을 한 번의 `push`로 넣으려고 하면 인코딩 범위를 벗어날 수 있습니다.

### Fix

문자열을 4바이트 단위로 나누거나, 코드 끝에 `db`로 데이터를 배치한 뒤 주소를 참조하는 방식을 사용합니다.

```asm
BITS 32
section .text
message:
    db '/bin/ps', 0
```

문자열의 바이트 수와 실제 저장 순서를 확인합니다.

## 4. Incorrect Escape Sequence

### Symptom

```text
warning: unknown escape sequence: '\X'
```

### Cause

C 바이트 문자열의 `\x`를 대문자 `\X`로 입력했습니다. 이 경우 의도한 기계어 바이트가 만들어지지 않습니다.

### Fix

```c
unsigned char bytes[] = "\x31\xc0\xb0\x0b";
```

C 문자열은 `\x` 다음에 16진수 숫자가 와야 합니다. 어셈블리로 생성한 원본 바이트와 C 배열을 비교합니다.

## 5. Segmentation Fault

### Symptom

```text
Segmentation fault
```

### Possible Causes

- 기계어 바이트가 잘못 변환됨
- 잘못된 주소를 함수 포인터로 호출함
- 실행 불가능한 메모리에서 명령어 실행 시도
- `argv` 또는 경로 포인터가 잘못 구성됨
- 문자열의 종료 바이트가 누락됨

### Investigation

1. `ndisasm`으로 명령어 바이트를 확인합니다.
2. C 문자열의 `\xNN`을 바이트 단위로 대조합니다.
3. 실행 메모리 권한을 확인합니다.
4. `execve` 인자 포인터 배열과 NULL 종료를 확인합니다.
5. 컴파일 경고가 있는지 확인합니다.

## 6. Executable Stack Warning

### Symptom

```text
warning: -z execstack ignored
```

### Explanation

해당 링커가 `-z execstack` 옵션을 적용하지 않았다는 의미입니다. 실행 스택이 필요하다는 설정과 실제 코드가 실행되는 메모리 영역은 구분해야 합니다.

### Approach

실습 C 래퍼는 `mmap`으로 실행 가능한 메모리를 할당해 바이트를 복사했습니다. 따라서 해당 경고만으로 프로그램의 실행 성공/실패를 판단하지 않고 실제 출력으로 확인했습니다.

## 7. Shellcode Length Mismatch

### Symptom

예상과 다른 `Shellcode size` 출력이 나타납니다.

### Causes

- C 문자열의 자동 NUL 종료를 포함해 계산함
- 바이트가 누락되거나 추가됨
- 잘못된 이스케이프 시퀀스로 바이트 배열이 변경됨

### Fix

```c
size_t len = sizeof(shellcode) - 1;
printf("Shellcode size: %zu bytes\n", len);
```

`strlen`은 중간 NUL 바이트가 있는 바이너리 데이터에 적합하지 않습니다.

## 8. Output Does Not Match Command

### Example

`whoami` 실습에서 크기만 출력되거나, `cat` 실행 시 기대한 파일 내용이 나오지 않는 경우입니다.

### Investigation

- 프로그램 경로의 철자와 슬래시 확인
- 인자 배열의 첫 번째 요소가 실행 경로인지 확인
- 두 번째 인자가 올바른 파일 경로인지 확인
- 포인터 배열이 NULL로 끝나는지 확인
- `execve` 실패 시 오류를 출력하도록 C 래퍼 구성

## 9. General Debugging Checklist

```bash
pwd
ls -l
file whoS
readelf -h whoS
ndisasm -b32 who
```

오류를 해결할 때는 한 번에 여러 부분을 수정하기보다, 소스 저장 → 어셈블 → 바이트 변환 → C 컴파일 → 실행의 단계별 결과를 비교하는 방식이 효과적이었습니다.
