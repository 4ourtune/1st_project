#include "byte_queue.h"

int ByteQueue_Init (ByteQueue *q, int capacity)
{
    if (!q || capacity <= 0 || capacity > MAX_QUEUE_SIZE)
        return 0;

    q->head = 0;
    q->tail = 0;
    q->capacity = capacity;

    return 1;
}

int ByteQueue_Push (ByteQueue *q, uint8_t byte)
{
    if (q->capacity <= 0)
        return 0;

    q->buffer[q->tail] = byte;
    q->tail = (q->tail + 1) % q->capacity;

    if (q->tail == q->head)  // 큐가 가득 찬 경우
    {
        q->head = (q->head + 1) % q->capacity; // 가장 오래된 항목 삭제
    }

    return 1;
}

int ByteQueue_Pop (ByteQueue *q, uint8_t *byte)
{
    if (q->capacity <= 0 || q->head == q->tail) // 큐가 비어있는지 확인
        return 0;

    *byte = q->buffer[q->head];
    q->head = (q->head + 1) % q->capacity;
    return 1;
}
