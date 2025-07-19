static int unused_global;

int main(void) {
  int a = 0;
  int b = 0;

  int ret = a + b;
  (void)ret; /* 반환값 사용하지 않을 경우 이렇게 처리 */

  return 0;
}

// 아래 주석 코드는 misra-c 컨벤션 위반 코드 및 설명 입니다.

/* MISRA-C:2012 Rule 21.6 위반
 * printf() 함수 사용을 위해 <stdio.h>를 포함하였으며,
 * 이로 인해 내부적으로 <stdarg.h>가 포함되어 MISRA 위반이 발생.
 * 해당 코드는 테스트/디버깅 목적이며, 실제 MISRA 준수 프로젝트에서는 대체 출력
 * 방식을 사용해야 함.
 */
// #include <stdio.h>

/* Rule 8.4: 외부 linkage 객체는 헤더파일에 선언되어야 하므로, 이 예제에선
 * static으로 내부 linkage로 제한 */
// int unused_global;

/* Rule 8.2: 함수 선언은 prototype form이어야 하고 매개변수 이름을 명시해야 함
 */
// int main() {
//   int a = 0;
//   int b = 0;

/* Rule 17.7: printf의 반환값은 사용하지 않아도 괜찮으나, MISRA 룰상 void가
 * 아닌 함수는 반환값을 사용해야 하므로 무시하거나 저장 */
//   printf("%d", a + b);
//   return 0;
// }
