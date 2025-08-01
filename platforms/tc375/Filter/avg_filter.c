#include "avg_filter.h"

int Filter_Init (AverageFilter *filter, int size)
{
    if (size <= 0 || size > MAX_FILTER_SIZE)
        return -1;

    for (int i = 0; i < MAX_FILTER_SIZE; i++)
        filter->buf[i] = 0.0f;

    filter->filter_size = size;
    filter->next_index = 0;
    filter->data_cnt = 0;

    return 0;
}

float Filter_Update (AverageFilter *filter, float new_value)
{
    if (filter->filter_size <= 0 || filter->filter_size > MAX_FILTER_SIZE)
        return 0.0f;

    filter->buf[filter->next_index] = new_value;
    filter->next_index = (filter->next_index + 1) % filter->filter_size;

    if (filter->data_cnt < filter->filter_size)
        filter->data_cnt++;

    float sum = 0.0f;
    for (int i = 0; i < filter->data_cnt; i++)
        sum += filter->buf[i];

    return sum / filter->data_cnt;
}

void Filter_Reset (AverageFilter *filter)
{
    for (int i = 0; i < MAX_FILTER_SIZE; i++)
        filter->buf[i] = 0.0f;

    filter->next_index = 0;
    filter->data_cnt = 0;
}
