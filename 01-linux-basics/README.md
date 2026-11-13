# Linux Permissions & SetUID

## 학습 주제

- Linux 파일 권한: owner / group / others
- `chmod` 명령어와 권한 비트
- Real UID와 Effective UID
- SetUID / SetGID 동작 원리
- 프로그램 실행 시 권한과 프로세스의 관계

## 주요 명령어

```bash
id
ls -l
chmod 755 example
stat example
```

## 정리

Linux의 접근 제어는 사용자와 그룹, 파일의 권한 비트에 따라 결정됩니다. SetUID가 설정된 실행 파일은 실행 과정에서 소유자의 Effective UID로 동작할 수 있으므로, 불필요한 권한 부여를 피하고 입력과 실행 경로를 검증해야 합니다.
