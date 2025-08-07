#ifndef BSW_IO_TOF_H_
#define BSW_IO_TOF_H_

#include <stdint.h>
#include <stdbool.h>
#include "byte_queue.h"

#include "stm.h"

#define TOF_FRAME_LENGTH 16
#define TOF_FRAME_HEADER 0x57

#define TOF_BUFFER_SIZE 64

typedef struct
{
    uint8_t id;
    uint32_t system_time_ms;
    float distance_m;
    uint8_t distance_status;
    uint16_t signal_strength;
    uint64_t received_time_us;
} ToFData_t;

// 초기화
void ToF_Init (void);

// 인터럽트에서 호출
void ToF_RxHandler (uint8_t byte);

// 메인 루프에서 호출
void ToF_ProcessQueue (void);

// 최신 데이터 가져오기
bool ToF_GetLatestData (ToFData_t *out);

#endif /* BSW_IO_TOF_H_ */
