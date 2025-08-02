#include "str_queue.h"

#define QUEUE_SIZE 64
#define NEXT(idx) (((idx) + 1) % QUEUE_SIZE)

static char buffer[QUEUE_SIZE];
static int head = 0; // Read 위치
static int tail = 0; // Write 위치

void queue_push_char (char ch)
{
    buffer[tail] = ch;
    tail = NEXT(tail);
    if (head == tail) // 버퍼 가득참
    {
        head = NEXT(head); // 가장 오래된 데이터 덮어쓰기 위해 head 이동
    }
}

int queue_pull_char (char *ch)
{
    if (head == tail) // 큐 비어있음
        return 0;
    *ch = buffer[head];
    head = NEXT(head);
    return 1;
}

int queue_pull_string (char *dst, int max_len)
{
    static int cur_len = 0;

    if (queue_pull_char(dst + cur_len))
    {
        cur_len++;

        if (dst[cur_len - 1] == '\0') // 버퍼에 담아오다가 널문자 받으면 완성 출력
        {
            cur_len = 0;
            return 1;
        }
        else if (cur_len >= max_len) // 버퍼 크기 초과시 flush
        {
            cur_len = 0;
        }
    }

    return 0;
}
