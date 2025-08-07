#ifndef UTILS_QUEUE_BYTE_QUEUE_H_
#define UTILS_QUEUE_BYTE_QUEUE_H_

#include <stdint.h>

#define MAX_QUEUE_SIZE 64

typedef struct
{
    uint8_t buffer[MAX_QUEUE_SIZE]; // 고정 크기 버퍼
    int head;       // 읽기 위치
    int tail;       // 쓰기 위치
    int capacity;   // 현재 사용하는 버퍼 크기 (<= MAX_QUEUE_SIZE)
} ByteQueue;

int ByteQueue_Init (ByteQueue *q, int capacity);
int ByteQueue_Push (ByteQueue *q, uint8_t byte);
int ByteQueue_Pop (ByteQueue *q, uint8_t *byte);

#endif /* UTILS_QUEUE_BYTE_QUEUE_H_ */
