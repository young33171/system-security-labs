# link & chmod System Calls

## 학습 주제

Linux의 파일 시스템 관련 시스템 콜인 `link`와 `chmod`를 x86 어셈블리 관점에서 살펴봅니다.

- `link(oldpath, newpath)`: 기존 파일에 하드 링크 생성
- `chmod(path, mode)`: 파일 권한 변경

## 주요 개념

`link`는 같은 파일 시스템 내에서 하나의 inode를 여러 경로가 참조하도록 합니다. `chmod`는 파일의 접근 권한을 변경합니다.

시스템 콜을 어셈블리로 호출할 때는 호출 번호와 인자 레지스터, 반환값 및 오류 처리를 확인해야 합니다.
