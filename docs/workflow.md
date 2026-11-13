# 실습 워크플로

1. Geany에서 `NAME.s` 작성 및 Ctrl+S 저장
2. `nasm -f bin NAME.s -o NAME` — NASM flat binary 생성
3. `ndisasm -b32 NAME > dump_NAME` — 32-bit 디스어셈블
4. `perl /root/convert.pl dump_NAME out_NAME` — 바이트열 변환
5. `out_NAME`의 바이트를 실제 `NAME.c`의 `shellcode[]`와 비교
6. `gcc -m32 -z execstack -o NAMES NAME.c` — 32-bit C 프로그램 컴파일
7. `./NAMES` — 실제 프로그램의 동작 결과 검증
8. 터미널 스크린샷 저장, 안전 검토 후 evidence 폴더에 업로드

## 환경 경로 오류
NASM 원본은 `/root`에 있는 경우가 있고 C 원본은 `/media/sf_SystemSecurity` 공유 폴더에 있는 경우가 있었습니다. 반드시 `pwd`, `ls`를 사용해 현재 폴더에서 파일이 존재하는지 확인합니다.

## C 실행 래퍼
해당 Kali 환경에서 `-z execstack ignored` 경고가 발생했습니다. 수업에서는 실습용으로 `mmap(PROT_READ | PROT_WRITE | PROT_EXEC)`으로 메모리를 할당해 변환된 바이트를 복사한 뒤 함수 포인터로 호출했습니다. 이 기법은 실서비스에서 허용되지 않습니다.
