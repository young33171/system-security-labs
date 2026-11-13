# Linux 시스템보안 기초

## 목표
- 파일 권한: user, group, other의 r/w/x 비트
- `chmod`와 특수권한: SetUID(SUID), SetGID(SGID)
- Real UID / Effective UID 구분
- 프로세스 실행과 스택, 코드·데이터 영역의 관계

## 실습 기록
과거 터미널 화면에서 `setuid.c`, `chmod 4755`, `gcc -o setuid ./setuid.c` 등이 언급되었습니다. 하지만 **원본 C 파일과 실행 결과는 아직 확보되지 않았습니다.** 실습 결과나 권한 상승을 임의로 주장하지 않습니다.

## 재검증 목록
- id, ls -l 명령 결과
- SetUID 프로그램의 소유자와 실행 전후 UID/EUID
- 권한 변경 실험 전후 파일 메타데이터
- 오용 위험과 예방: 최소 권한, 경로 검증, 환경 변수 관리

> 실제 개인 VM에서만 재현하고 권한 상승 기법을 실서비스나 타인의 컴퓨터에 사용하지 않습니다.
