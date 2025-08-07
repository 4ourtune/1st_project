#include "Ultrasonic.h"

#define FILTER_WINDOW_SIZE 5
#define CALCULATE_DISTANCE(fall_time, rise_time) (((int32_t)((fall_time) - (rise_time)) * 343) / 2000) // Speed of sound : 0.343 mm/us

static AverageFilter filters[ULTRASONIC_COUNT];
static UltrasonicData_t ult_data[ULTRASONIC_COUNT];
static uint64_t rise_time[ULTRASONIC_COUNT];
static bool data_ready[ULTRASONIC_COUNT] = {false};

void Ultrasonic_Init (void)
{
    MODULE_P10.IOCR4.B.PC4 = 0x10; /* Set P10.4 as output */
    MODULE_P10.OUT.B.P4 = 0;

    for (int i = 0; i < ULTRASONIC_COUNT; ++i)
        Filter_Init(&filters[i], FILTER_WINDOW_SIZE);
}

void Ultrasonic_Toggle_Trigger (void)
{
    static int cnt = 0;
    /* Ultrasonic sensor: Set the period to 40ms. 38ms(Max echo back pulse duration) + 2ms(Margin including trigger pulse) */
    cnt = (cnt + 1) % 40;

    if (cnt == 1 || cnt == 2)
        MODULE_P10.OUT.B.P4 = !MODULE_P10.OUT.B.P4;
}

void Ultrasonic_EchoHandler (UltrasonicSide side, int input_pin_state)
{
    if (side >= ULTRASONIC_COUNT)
        return;

    if (input_pin_state)
    {
        rise_time[side] = getTimeUs();
    }
    else
    {
        uint64_t fall_time = getTimeUs();
        int32_t raw_dist = CALCULATE_DISTANCE(fall_time, rise_time[side]);
        int32_t filt_dist = Filter_Update(&filters[side], raw_dist);

        ult_data[side].distance_mm = (int) filt_dist;
        ult_data[side].timestamp_us = rise_time[side];
        data_ready[side] = true;
    }
}

bool Ultrasonic_GetLatestData (UltrasonicSide side, UltrasonicData_t *out)
{
    if (side >= ULTRASONIC_COUNT || !out)
        return false;

    if (!data_ready[side])
        return false;

    *out = ult_data[side];
//    data_ready[side] = false; // 읽으면 비움 (필요시 주석 해제)
    return true;
}
