# 증거 기반 검증 체크리스트

- [x] 1번 who: tty7 / pts 목록 화면 확인
- [x] 2번 whoami: Shellcode size 39 bytes → root 출력 화면 확인
- [x] 3번 cat passwd: /etc/passwd 계정 목록 출력 화면 확인
- [x] 4번 cat hosts: Shellcode size 52 bytes 및 localhost/kali 출력 확인
- [ ] 5번 ps: NASM/ndisasm/convert.pl 실행 화면 확인, 최종 프로세스 목록 화면 미확인
- [ ] 6번 ps -aef: 과제용 코드 안내는 있었지만 실제 실습 실행 결과 미확인
- [ ] 7번 link/chmod: 과제 안내만 확인
- [ ] VM에 남아 있는 실제 convert.pl 확보
- [ ] VM 원본 .s, .c, dump_*, out_*와 복원본 대조
- [ ] SetUID, stack_example, buf_ret 등의 원본 코드/실행 캡처 확보
- [ ] 화면 캡처 개인정보 마스킹 후 evidence 추가

최종 수정일: 2026-11-13. 이 목록은 제출/성공으로 오인하지 않도록 관찰된 출력 기준으로 기록했습니다.
