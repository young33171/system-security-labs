# 직접 겪은 문제와 해결

| 증상 | 원인 | 해결 |
|---|---|---|
| `who.c: No such file or directory` | /root 와 공유 폴더의 경로 혼동 | pwd/ls 확인 후 파일이 있는 폴더로 이동 |
| `readlf: command not found` | readelf 오타 | `readelf -W -l whoS` (끝은 소문자 L) |
| `dword data exceeds bounds` | 32비트 push 즉시값 초과 | NASM 데이터 문자열/jmp-call-pop 형태 사용 |
| `unknown escape sequence: '\X'` | C 문자열에 잘못된 대문자 X | 소문자 `\xNN` 사용 |
| whoami 111 bytes / Segmentation fault | 잘못된 C 문자열 이스케이프 | 올바른 변환 값으로 고친 뒤 39 bytes와 root 출력 확인 |
| `-z execstack ignored` | 환경에서 해당 링커 옵션 무시 | 경고와 별개로 실행 메모리 및 실제 동작 검증 |
| `Shellcode size: 30 bytes`만 출력 | 프로그램이 길이만 출력함 | C 래퍼에서 실제 호출 여부 확인 |
| Segmentation fault | 잘못된 바이트 또는 비실행 메모리 | 원본 바이트/메모리 권한 점검 |
| /usr/bin/who 실행 오류 | 문자열의 슬래시 배치 오류 | `/usr` + `/bin` + `/who` 경로 확인 |
| 터미널 종료 | VM 터미널 세션 닫힘 | Ctrl+Alt+T, 작업 파일 저장 여부 재확인 |
| 컴파일은 되지만 파일 실행 불가 | 바이너리 경로/권한/옵션 문제 | `ls -l`, `file`, `pwd` 점검 |

## 검증 원칙
컴파일 통과와 실제 실행 성공은 다릅니다. `whoami`는 root 출력, `who`는 접속 목록, `cat`은 지정 파일 내용, `ps`는 프로세스 표를 확인해야 성공입니다.
