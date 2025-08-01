#include "aps.h"
#include "my_stdio.h"

#define CYCLE_DELAY_US 100000 // 100000us = 100ms
#define SENSOR_DATA_COUNT 4

static volatile int aps_state = 0;
static unsigned int sense_dist[SENSOR_DATA_COUNT]; // 0: ToF_front, 1: Ult_left, 2: Ult_right, 3: Ult_rear
static uint64 sense_time[SENSOR_DATA_COUNT];

int Calc_APS_Result (int *res_x, int *res_y)
{
    // APS logic below

    return 0;
}

int Get_APS_State (void)
{
    return aps_state;
}

void Set_APS_State (int state)
{
    aps_state = state;
}

int Get_APS_Result (int *res_x, int *res_y)
{
    static uint64 pre_time = 0;
    uint64 cur_time = getTimeUs();

    if (cur_time - pre_time < CYCLE_DELAY_US)
        return 0;

    pre_time = cur_time;

    unsigned short tof_signal;
    unsigned char tof_status;
    Tof_GetDistance(sense_dist + 0, &tof_signal, &tof_status, sense_time + 0); // ToF_front
    get_eru_int0_data(sense_dist + 1, sense_time + 1); // Ult_left
    get_eru_int1_data(sense_dist + 2, sense_time + 2); // Ult_right
    get_eru_int2_data(sense_dist + 3, sense_time + 3); // Ult_rear

    cur_time = getTimeUs();

    for (int i = 0; i < SENSOR_DATA_COUNT; i++)
    {
        uint64 sensor_delay = cur_time - sense_time[i];

        if (sensor_delay > CYCLE_DELAY_US)
            return 0; // If there is any timeout data, return 0
    }

    return Calc_APS_Result(res_x, res_y);
}
