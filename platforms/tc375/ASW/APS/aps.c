#include "aps.h"

#define SENSOR_DATA_COUNT 4

static unsigned int sense_dist[SENSOR_DATA_COUNT]; // 0: ToF_front, 1: Ult_left, 2: Ult_right, 3: Ult_rear
static uint64 sense_time[SENSOR_DATA_COUNT];

static volatile int aps_state = 0;
static int result_x;
static int result_y;
static int is_APS_done;

void Calc_APS_Result (void)
{
    // APS logic below

}

int Update_APS_Result (uint64 interval_us)
{
    unsigned short tof_signal;
    unsigned char tof_status;
    Tof_GetDistance(sense_dist + 0, &tof_signal, &tof_status, sense_time + 0); // ToF_front
    Ultrasonic_Get_Left_Data(sense_dist + 1, sense_time + 1); // Ult_left
    Ultrasonic_Get_Right_Data(sense_dist + 2, sense_time + 2); // Ult_right
    Ultrasonic_Get_Rear_Data(sense_dist + 3, sense_time + 3); // Ult_rear

    uint64 cur_time = getTimeUs();

    for (int i = 0; i < SENSOR_DATA_COUNT; i++)
    {
        uint64 sensor_delay = cur_time - sense_time[i];

        if (sensor_delay > interval_us)
            return 0; // If there is any timeout sensor data, return 0
    }

    Calc_APS_Result();
    return 1;
}

void Get_APS_Result (int *res_x, int *res_y, int *is_done)
{
    *res_x = result_x;
    *res_y = result_y;
    *is_done = is_APS_done;
}

int Get_APS_State (void)
{
    return aps_state;
}

void Set_APS_State (int state)
{
    aps_state = state;
}
