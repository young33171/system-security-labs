# Troubleshooting

실습 과정에서 발생한 오류와 해결 방법을 정리했습니다.

| Error | Cause | Fix |
|---|---|---|
| `No such file or directory` | 현재 작업 폴더와 소스 위치가 다름 | `pwd`, `ls`로 경로 확인 |
| `readlf: command not found` | 명령어 오타 | `readelf` 사용 |
| NASM immediate exceeds bounds | 32비트 범위를 넘는 상수 | 문자열 데이터 배치 방식 수정 |
| `unknown escape sequence: '\X'` | C 바이트 문자열에서 대문자 X 사용 | 소문자 `\xNN`로 수정 |
| `Segmentation fault` | 바이트 오류 또는 메모리 실행 권한 문제 | 쉘코드 바이트와 메모리 권한 점검 |
| `-z execstack ignored` | 링커 옵션 적용 문제 | 실행 권한 설정과 결과를 별도 확인 |
| 쉘코드 크기만 출력 | C 프로그램에서 함수 호출을 하지 않음 | 쉘코드 호출 코드 확인 |
| 잘못된 실행 경로 | 어셈블리 문자열의 슬래시 배치 오류 | 경로 문자열을 바이트 단위로 점검 |

## Lessons Learned

- 컴파일 성공과 프로그램 실행 성공은 별개입니다.
- `\xNN` 문자열의 한 글자 오류도 전혀 다른 바이트를 만듭니다.
- `/root`와 공유 폴더를 혼용할 때 파일 위치를 먼저 확인해야 합니다.
- 실행 경로, NUL 종료, 인자 배열, 메모리 권한을 함께 점검해야 합니다.
