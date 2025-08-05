#ifndef UTILS_FILTER_AVG_FILTER_H_
#define UTILS_FILTER_AVG_FILTER_H_

#include <stdint.h>

#define MAX_FILTER_SIZE 20

typedef struct
{
    int32_t buf[MAX_FILTER_SIZE];  // 고정된 정수 버퍼
    int filter_size;               // 사용 중인 필터 크기
    int next_index;                // 다음 인덱스
    int data_cnt;                  // 유효 데이터 수
    int64_t sum;                   // 누적합 (오버플로우 방지용)
} AverageFilter;

/* 필터 초기화 (size: 1~MAX_FILTER_SIZE) */
int Filter_Init (AverageFilter *filter, int size);

/* 새 값을 추가하고 정수 평균 반환 */
int32_t Filter_Update (AverageFilter *filter, int32_t new_value);

/* 필터 리셋 */
int Filter_Reset (AverageFilter *filter);

#endif /* UTILS_FILTER_AVG_FILTER_H_ */
