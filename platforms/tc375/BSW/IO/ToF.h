#ifndef BSW_IO_TOF_H_
#define BSW_IO_TOF_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm.h"

#define TOF_FRAME_LENGTH 16
#define TOF_FRAME_HEADER 0x57

typedef struct
{
    uint8_t id;
    uint32_t system_time_ms;
    float distance_m;
    uint8_t distance_status;
    uint16_t signal_strength;
    uint64_t stm0_time_us;
} ToFData_t;

// 수신 처리 함수
void ToF_RxHandler (uint8_t ch);

// 수신된 최신 데이터 가져오기
bool ToF_GetLatestData (ToFData_t *out);

#endif /* BSW_IO_TOF_H_ */
