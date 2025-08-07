#ifndef BSW_IO_BLUETOOTH_H_
#define BSW_IO_BLUETOOTH_H_

#include "byte_queue.h"

#define BLUETOOTH_BUFFER_SIZE 64

void Bluetooth_Init (void);
int Bluetooth_RxHandler (uint8_t byte);
int Bluetooth_RxQueue_PopString (char *dst, int dst_size);

#endif /* BSW_IO_BLUETOOTH_H_ */
