#include "avg_filter.h"

int Filter_Init (AverageFilter *filter, int size)
{
    if (!filter || size <= 0 || size > MAX_FILTER_SIZE)
        return 0;

    for (int i = 0; i < size; i++)
        filter->buf[i] = 0;

    filter->filter_size = size;
    filter->next_index = 0;
    filter->data_cnt = 0;
    filter->sum = 0;

    return 1;
}

int32_t Filter_Update (AverageFilter *filter, int32_t new_value)
{
    if (!filter || filter->filter_size <= 0 || filter->filter_size > MAX_FILTER_SIZE)
        return 0;

    int idx = filter->next_index;
    int32_t old_value = filter->buf[idx];

    if (filter->data_cnt < filter->filter_size)
        filter->data_cnt++;
    else
        filter->sum -= old_value;

    filter->buf[idx] = new_value;
    filter->sum += new_value;
    filter->next_index = (idx + 1) % filter->filter_size;

    return (int32_t) (filter->sum / filter->data_cnt);
}

int Filter_Reset (AverageFilter *filter)
{
    if (!filter)
        return 0;

    for (int i = 0; i < filter->filter_size; i++)
        filter->buf[i] = 0;

    filter->next_index = 0;
    filter->data_cnt = 0;
    filter->sum = 0;

    return 1;
}
