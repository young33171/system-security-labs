# Lab Workflow

## 1. Assembly

Geany에서 NASM 소스를 작성하고 저장합니다.

```bash
geany who.s
nasm -f bin who.s -o who
```

## 2. Disassembly

```bash
ndisasm -b32 who > dump_w
cat dump_w
```

## 3. Convert to C Bytes

```bash
perl /root/convert.pl dump_w out_w
cat out_w
```

## 4. Compile & Run

변환한 바이트를 C 프로그램의 `shellcode[]` 배열에 넣고 컴파일합니다.

```bash
gcc -m32 -z execstack -o whoS who.c
./whoS
```

## Environment

- Kali Linux 32-bit
- NASM / NDISASM
- GCC with `-m32`
- Geany
- VirtualBox 공유 폴더 `/media/sf_SystemSecurity`

### Implementation Note

실습 환경에서는 `-z execstack ignored` 경고가 발생했습니다. 실행 가능한 메모리 할당과 C 배열의 실제 바이트를 확인해 동작을 점검했습니다.
