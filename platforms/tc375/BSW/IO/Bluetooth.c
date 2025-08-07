#include "Bluetooth.h"

static ByteQueue bluetooth_rx_queue;
static int cur_str_len = 0; // 현재 수신 중인 문자열 길이

void Bluetooth_Init (void)
{
    ByteQueue_Init(&bluetooth_rx_queue, BLUETOOTH_BUFFER_SIZE);
    cur_str_len = 0;
}

int Bluetooth_RxHandler (uint8_t byte)
{
    if (byte == '\n') // 필터링
        return 0;

    ByteQueue_Push(&bluetooth_rx_queue, byte);
    return 1;
}

int Bluetooth_RxQueue_PopString (char *dst, int dst_size)
{
    uint8_t byte;

    if (ByteQueue_Pop(&bluetooth_rx_queue, &byte))
    {
        dst[cur_str_len++] = (char) byte;

        if (byte == '\0') // 문자열 끝
        {
            cur_str_len = 0;
            return 1;
        }
        else if (cur_str_len >= dst_size) // 버퍼 크기 초과
        {
            cur_str_len = 0;
            dst[0] = '\0';
        }
    }

    return 0;
}
