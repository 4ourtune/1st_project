#ifndef UTILS_STREAM_STR_QUEUE_H_
#define UTILS_STREAM_STR_QUEUE_H_

#define QUEUE_SIZE 64

typedef struct
{
    char buffer[QUEUE_SIZE]; // 데이터를 저장할 버퍼
    int head;                // 읽기 위치 (큐의 시작)
    int tail;                // 쓰기 위치 (큐의 끝)
    int cur_str_len;  // str_queue_pull_string을 위한 현재 문자열 길이
} str_queue_t;

void str_queue_init (str_queue_t *q);
void str_queue_push_char (str_queue_t *q, char ch);
//int str_queue_pull_char (str_queue_t *q, char *ch);
int str_queue_pull_string (str_queue_t *q, char *dst, int dst_size);

#endif /* UTILS_STREAM_STR_QUEUE_H_ */
