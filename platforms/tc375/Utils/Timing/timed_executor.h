#ifndef UTILS_TIMING_TIMED_EXECUTOR_H_
#define UTILS_TIMING_TIMED_EXECUTOR_H_

#include "stm.h"

// interval_us(마이크로초) 간격으로 func 호출
int call_after_interval (int (*func) (uint64), uint64 *last_called_time_us, uint64 interval_us);

#endif /* UTILS_TIMING_TIMED_EXECUTOR_H_ */
