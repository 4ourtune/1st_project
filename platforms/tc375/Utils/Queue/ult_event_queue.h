#ifndef UTILS_QUEUE_ULT_EVENT_QUEUE_H_
#define UTILS_QUEUE_ULT_EVENT_QUEUE_H_

#include <stdint.h>

#define ULTRASONIC_QUEUE_SIZE 16

typedef struct
{
    uint64_t rise_time;
    uint64_t fall_time;
} UltrasonicEvent;

typedef struct
{
    UltrasonicEvent buffer[ULTRASONIC_QUEUE_SIZE];
    int head;
    int tail;
    int capacity;
} UltrasonicEventQueue;

int UltEventQueue_Init (UltrasonicEventQueue *q, int capacity);
int UltEventQueue_Push (UltrasonicEventQueue *q, const UltrasonicEvent *event);
int UltEventQueue_Pop (UltrasonicEventQueue *q, UltrasonicEvent *out);

#endif /* UTILS_QUEUE_ULT_EVENT_QUEUE_H_ */
