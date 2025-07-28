#ifndef STREAM_STR_QUEUE_H_
#define STREAM_STR_QUEUE_H_

void queue_push_char (char ch);
//int queue_pull_char (char *ch);
int queue_pull_string (char *dst, int max_len);

#endif /* STREAM_STR_QUEUE_H_ */
