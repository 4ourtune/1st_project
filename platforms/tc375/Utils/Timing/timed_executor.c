#include "timed_executor.h"

int call_after_interval (int (*func) (uint64), uint64 *last_called_time_us, uint64 interval_us)
{
    if (!func || !last_called_time_us)
        return 0;

    uint64 current_time_us = getTimeUs();

    if ((current_time_us - *last_called_time_us) < interval_us)
        return 0;  // 실행 안됨

    *last_called_time_us = current_time_us;

    return func(interval_us); // 실행 결과 리턴
}
