#include "Bluetooth.h"

static str_queue_t bluetooth_rx_queue;

void bluetooth_rx_queue_init (void)
{
    str_queue_init(&bluetooth_rx_queue);
}

int bluetooth_rx_queue_push_char (char ch)
{
    if (ch == '\n')
        return 0;
    str_queue_push_char(&bluetooth_rx_queue, ch);
    return 1;
}

int bluetooth_rx_queue_pull_string (char *dst, int dst_size)
{
    return str_queue_pull_string(&bluetooth_rx_queue, dst, dst_size);
}
