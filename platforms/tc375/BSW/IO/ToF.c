#include "ToF.h"

// UART 수신 상태 관리용
static uint8_t rx_buffer[TOF_FRAME_LENGTH];
static uint8_t rx_index = 0;
static bool syncing = false;

static ToFData_t latest_data;
static bool data_ready = false;

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

void ToF_RxHandler (uint8_t ch)
{
    if (!syncing)
    {
        if (ch == TOF_FRAME_HEADER)
        {
            rx_index = 0;
            rx_buffer[rx_index++] = ch;
            syncing = true;
        }
    }
    else
    {
        rx_buffer[rx_index++] = ch;
        if (rx_index >= TOF_FRAME_LENGTH)
        {
            syncing = false;
            if (parseToFPacket(rx_buffer, &latest_data))
            {
                latest_data.stm0_time_us = getTimeUs();
                data_ready = true;
            }
            rx_index = 0;
        }
    }
}

bool ToF_GetLatestData (ToFData_t *out)
{
    if (!data_ready)
        return false;
    *out = latest_data;
//    data_ready = false; // 읽으면 비움 (필요시 주석 처리)
    return true;
}
