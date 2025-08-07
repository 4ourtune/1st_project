#include "ToF.h"

static ByteQueue rx_queue;
static uint64 sync_start_time_us[TOF_BUFFER_SIZE];

static ToFData_t latest_data;
static bool data_ready = false;

void ToF_Init (void)
{
    ByteQueue_Init(&rx_queue, TOF_BUFFER_SIZE);
}

void ToF_RxHandler (uint8_t byte)
{
    int tail = rx_queue.tail;

    ByteQueue_Push(&rx_queue, byte);

    if (byte == TOF_FRAME_HEADER)
    {
        sync_start_time_us[tail] = getTimeUs();
    }
}

static bool verifyCheckSum (const uint8_t *data, int32_t length)
{
    uint8_t sum = 0;
    for (int32_t i = 0; i < length - 1; ++i)
    {
        sum += data[i];
    }
    return sum == data[length - 1];
}

static bool parseToFPacket (const uint8_t *packet, ToFData_t *out)
{
    if (packet[0] != TOF_FRAME_HEADER)
        return false;
    if (!verifyCheckSum(packet, TOF_FRAME_LENGTH))
        return false;

    out->id = packet[3];

    out->system_time_ms = ((uint32_t) packet[4]) | ((uint32_t) packet[5] << 8) | ((uint32_t) packet[6] << 16)
            | ((uint32_t) packet[7] << 24);

    int32_t dist_raw = ((int32_t) (packet[8] << 8) | (int32_t) (packet[9] << 16) | (int32_t) (packet[10] << 24)) / 256;
    out->distance_m = dist_raw / 1000.0f;

    out->distance_status = packet[11];

    out->signal_strength = ((uint16_t) packet[12]) | ((uint16_t) packet[13] << 8);

    return true;
}

void ToF_ProcessQueue (void)
{
    static uint8_t packet[TOF_FRAME_LENGTH];
    static uint8_t index = 0;
    static bool syncing = false;

    static uint64 frame_start_time_us = 0;

    uint8_t byte;
    while (ByteQueue_Pop(&rx_queue, &byte))
    {
        int head = (rx_queue.head - 1 + TOF_BUFFER_SIZE) % TOF_BUFFER_SIZE;

        if (!syncing)
        {
            if (byte == TOF_FRAME_HEADER)
            {
                index = 0;
                packet[index++] = byte;
                frame_start_time_us = sync_start_time_us[head];
                syncing = true;
            }
        }
        else
        {
            packet[index++] = byte;
            if (index >= TOF_FRAME_LENGTH)
            {
                syncing = false;
                index = 0;
                if (parseToFPacket(packet, &latest_data))
                {
                    latest_data.received_time_us = frame_start_time_us;
                    data_ready = true;
                }
            }
        }
    }
}

bool ToF_GetLatestData (ToFData_t *out)
{
    if (!data_ready)
        return false;

    *out = latest_data;
    data_ready = false; // 읽으면 비움 (필요시 주석 처리)
    return true;
}
