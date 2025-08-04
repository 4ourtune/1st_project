#include "ToF.h"

static unsigned int g_TofValue = 0;
static unsigned short g_SignalStrength = 0;
static unsigned char g_DisStatus = 0;
static uint64 g_TofTimestampUs = 0;

void TofHandler (unsigned char *rxData, int rxLen)
{
    // ToF sensor: The data update frequency is 30Hz(33.333..ms)
    g_TofTimestampUs = getTimeUs();
    g_TofValue = (rxData[2] << 16) | (rxData[1] << 8) | rxData[0];
    g_DisStatus = rxData[3];
    g_SignalStrength = (rxData[5] << 8) | rxData[4];
}

// 안전하게 거리 정보를 가져오는 함수
void Tof_GetDistance (unsigned int *dist_mm, unsigned short *signal, unsigned char *status, uint64 *timestamp_us)
{
//    __disable();
    *dist_mm = g_TofValue;
    *signal = g_SignalStrength;
    *status = g_DisStatus;
    *timestamp_us = g_TofTimestampUs;
//    __enable();
}

// 거리 보정 함수: 보정된 거리 값 리턴 (단위: mm)
unsigned int Tof_GetCorrectedDistance (void)
{
//    __disable();
    unsigned int raw = g_TofValue;
//    __enable();

    int corrected = (int) raw - 50; // 50mm 보정
    if (corrected < 0)
        corrected = 0;
    return (unsigned int) corrected;
}
