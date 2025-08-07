#include "Ultrasonic.h"

static UltrasonicEventQueue rx_queues[ULTRASONIC_COUNT];
static AverageFilter filters[ULTRASONIC_COUNT];
static UltrasonicData_t latest_data[ULTRASONIC_COUNT];
static bool data_ready[ULTRASONIC_COUNT] = {false};

void Ultrasonic_Init (void)
{
    for (int i = 0; i < ULTRASONIC_COUNT; ++i)
    {
        Filter_Init(&filters[i], ULT_FILTER_WINDOW_SIZE);
        UltEventQueue_Init(&rx_queues[i], ULTRASONIC_QUEUE_SIZE);
    }
}

void Ultrasonic_EchoHandler (UltrasonicSide side, int input_pin_state)
{
    static uint64_t rise_time_us[ULTRASONIC_COUNT];

    if (side >= ULTRASONIC_COUNT)
        return;

    uint64_t now = getTimeUs();

    if (input_pin_state)
    {
        rise_time_us[side] = now;
    }
    else
    {
        UltrasonicEvent evt = {.rise_time = rise_time_us[side], .fall_time = now};
        UltEventQueue_Push(&rx_queues[side], &evt);
    }
}

void Ultrasonic_ProcessQueue (void)
{
    for (int i = 0; i < ULTRASONIC_COUNT; ++i)
    {
        UltrasonicEvent evt;
        while (UltEventQueue_Pop(&rx_queues[i], &evt))
        {
            int32_t raw_dist = CALCULATE_DISTANCE(evt.fall_time, evt.rise_time);
            int32_t filtered = Filter_Update(&filters[i], raw_dist);

            latest_data[i].distance_mm = filtered;
            latest_data[i].received_time_us = evt.rise_time;
            data_ready[i] = true;
        }
    }
}

bool Ultrasonic_GetLatestData (UltrasonicSide side, UltrasonicData_t *out)
{
    if (side >= ULTRASONIC_COUNT)
        return false;

    if (!data_ready[side])
        return false;

    *out = latest_data[side];
    data_ready[side] = false; // 읽으면 비움 (필요시 주석 처리)
    return true;
}
