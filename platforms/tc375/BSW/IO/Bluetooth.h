#ifndef BSW_IO_BLUETOOTH_H_
#define BSW_IO_BLUETOOTH_H_

#include "str_queue.h"

void bluetooth_rx_queue_init (void);
int bluetooth_rx_queue_push_char (char ch);
int bluetooth_rx_queue_pull_string (char *dst, int dst_size);

#endif /* BSW_IO_BLUETOOTH_H_ */
