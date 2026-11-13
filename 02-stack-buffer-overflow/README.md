# 스택 구조 및 버퍼 오버플로우

## 실습 이력
화면에서 `stack_example.c`, `buf_ret.c`, `buf_ret`, `stack_example`, `~/bufferOverflow` 등이 관찰되었습니다. 스택 프레임의 매개변수, 지역변수, 저장된 프레임 포인터, 반환주소를 조사했습니다.

수업에서 `test_function(int a, int b, int c, int d)`, `flag = 31337`, `char buffer[10]`, `buffer[0] = 'A'` 예제를 다뤘습니다. **실제 원본 소스가 없는 부분은 임의로 재구성하여 원본으로 표시하지 않았습니다.**

## 핵심 개념
- Stack Frame: 함수 호출 시 저장되는 지역 변수, 반환 주소, 프레임 포인터
- Out-of-bounds Write: 버퍼 범위를 넘은 쓰기가 인접 메모리를 오염시킬 수 있음
- 방어: 길이 검사, 컴파일러 Stack Protector, NX, ASLR, PIE, 안전한 API

## 추가할 증빙
1. VM에 남은 실제 `stack_example.c`, `buf_ret.c` 등
2. 컴파일 옵션과 실제 실행 출력
3. 디버깅 화면과 주소/스택 레이아웃(민감정보 마스킹)
4. 오버플로우의 원인 분석, 방어 옵션 적용 결과 비교

현재 상태: 원본 확인 전. 실제로 검증하지 않은 공격 성공은 기록하지 않습니다.
