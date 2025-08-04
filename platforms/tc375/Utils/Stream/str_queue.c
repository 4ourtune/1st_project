#include "str_queue.h"

#define NEXT(idx) (((idx) + 1) % QUEUE_SIZE)

void str_queue_init (str_queue_t *q)
{
    q->head = 0;
    q->tail = 0;
    q->cur_str_len = 0;
}

void str_queue_push_char (str_queue_t *q, char ch)
{
    q->buffer[q->tail] = ch;
    q->tail = NEXT(q->tail);

    if (q->head == q->tail) // 큐가 가득 찬 경우
    {
        q->head = NEXT(q->head); // 가장 오래된 데이터를 덮어쓰기 위해 head를 이동
    }
}

int str_queue_pull_char (str_queue_t *q, char *ch)
{
    if (q->head == q->tail) // 큐가 비어있는지 확인
        return 0;

    *ch = q->buffer[q->head];
    q->head = NEXT(q->head);
    return 1;
}

int str_queue_pull_string (str_queue_t *q, char *dst, int dst_size)
{
    if (str_queue_pull_char(q, dst + q->cur_str_len))
    {
        q->cur_str_len++;

        if (dst[q->cur_str_len - 1] == '\0') // 문자열 끝
        {
            q->cur_str_len = 0;
            return 1;
        }
        else if (q->cur_str_len >= dst_size) // 버퍼 크기 초과
        {
            q->cur_str_len = 0;
        }
    }
    return 0;
}
