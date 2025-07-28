//eru.h
/*
 * scueru.h
 *
 *  Created on: 2024. 11. 6.
 *      Author: Kimjh
 */
#ifndef BSW_DRIVER_ERU_H_
#define BSW_DRIVER_ERU_H_
void scueru_Init(void);
void SCUERU_Int0_Handler(void);
void SCUERU_Int1_Handler(void);
void SCUERU_Int2_Handler(void);
typedef struct {
    uint64 start_time;
    uint64 end_time;
    uint64 duration;
    float  distance_cm;
    uint8  echo_state;
} UltrasonicSensor;
typedef struct {
    uint64 rear_sensor_start_time;
    uint64 rear_sensor_end_time;
    uint64 left_sensor_start_time;
    uint64 left_sensor_end_time;
    uint64 right_sensor_start_time;
    uint64 right_sensor_end_time;
} UltrasonicSensor_Info;

// 전역 변수 선언 (extern 키워드 사용)
volatile UltrasonicSensor rear_sensor;
volatile UltrasonicSensor right_sensor;
volatile UltrasonicSensor left_sensor;
volatile UltrasonicSensor_Info;

#endif /* BSW_DRIVER_ERU_H_ */
