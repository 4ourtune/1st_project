#ifndef FILTER_AVG_FILTER_H_
#define FILTER_AVG_FILTER_H_

#include <stdint.h>

#define MAX_FILTER_SIZE 20  // 버퍼 최대 크기

typedef struct
{
    float buf[MAX_FILTER_SIZE];  // 고정된 버퍼
    int filter_size;             // 실제 사용할 필터 크기
    int next_index;              // 다음 인덱스
    int data_cnt;                // 현재 유효 데이터 수
} AverageFilter;

/* 필터 초기화 - 사용할 필터 크기 설정 */
int Filter_Init (AverageFilter *filter, int size);

/* 새로운 값을 추가하고 필터링된 평균값 반환 */
float Filter_Update (AverageFilter *filter, float new_value);

/* 필터 리셋 (내용 초기화) */
void Filter_Reset (AverageFilter *filter);

#endif /* FILTER_AVG_FILTER_H_ */
