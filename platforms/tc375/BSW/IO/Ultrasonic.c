#include "Ultrasonic.h"

#define FILTER_WINDOW_SIZE 5
#define CALCULATE_DISTANCE(fall_time, rise_time) (0.343F * ((fall_time) - (rise_time)) / 2.0F) // Speed of sound: 0.343 mm/us

static AverageFilter filter[3];
static float ult_dist_mm[3];
static uint64 ult_timestamp_us[3];

void Ultrasonic_Init (void)
{
    MODULE_P10.IOCR4.B.PC4 = 0x10; /* Set P10.4 as output */
    MODULE_P10.OUT.B.P4 = 0;

    Filter_Init(filter + 0, FILTER_WINDOW_SIZE);
    Filter_Init(filter + 1, FILTER_WINDOW_SIZE);
    Filter_Init(filter + 2, FILTER_WINDOW_SIZE);
}

void Ultrasonic_Toggle_Trigger (void)
{
    static int ult_cnt = 0;

    /* Ultrasonic sensor: Set the period to 40ms. 38ms(Max echo back pulse duration) + 2ms(Margin including trigger pulse) */
    ult_cnt = (ult_cnt + 1) % 40;

    if (ult_cnt == 1 || ult_cnt == 2)
    {
        MODULE_P10.OUT.B.P4 = !MODULE_P10.OUT.B.P4;
    }
}

void Ultrasonic_Left_Handler (int input_pin_state)
{
    static uint64 rise_time;

    if (input_pin_state)
    {
        rise_time = getTimeUs();
    }
    else
    {
        uint64 fall_time = getTimeUs();
        float distance_nofilt = CALCULATE_DISTANCE(fall_time, rise_time);
        float distance_filt = Filter_Update(filter + 0, distance_nofilt);

        ult_dist_mm[0] = distance_filt;
        ult_timestamp_us[0] = rise_time;
    }
}

void Ultrasonic_Right_Handler (int input_pin_state)
{
    static uint64 rise_time;

    if (input_pin_state)
    {
        rise_time = getTimeUs();
    }
    else
    {
        uint64 fall_time = getTimeUs();
        float distance_nofilt = CALCULATE_DISTANCE(fall_time, rise_time);
        float distance_filt = Filter_Update(filter + 1, distance_nofilt);

        ult_dist_mm[1] = distance_filt;
        ult_timestamp_us[1] = rise_time;
    }
}

void Ultrasonic_Rear_Handler (int input_pin_state)
{
    static uint64 rise_time;

    if (input_pin_state)
    {
        rise_time = getTimeUs();
    }
    else
    {
        uint64 fall_time = getTimeUs();
        float distance_nofilt = CALCULATE_DISTANCE(fall_time, rise_time);
        float distance_filt = Filter_Update(filter + 2, distance_nofilt);

        ult_dist_mm[2] = distance_filt;
        ult_timestamp_us[2] = rise_time;
    }
}

void Ultrasonic_Get_Left_Data (unsigned int *dist_mm, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = (unsigned int) ult_dist_mm[0];
    *timestamp_us = ult_timestamp_us[0];
//    __enable();
}

void Ultrasonic_Get_Right_Data (unsigned int *dist_mm, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = (unsigned int) ult_dist_mm[1];
    *timestamp_us = ult_timestamp_us[1];
//    __enable();
}

void Ultrasonic_Get_Rear_Data (unsigned int *dist_mm, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = (unsigned int) ult_dist_mm[2];
    *timestamp_us = ult_timestamp_us[2];
//    __enable();
}
