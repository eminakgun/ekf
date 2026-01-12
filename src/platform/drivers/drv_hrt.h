#pragma once
#include <stdint.h>

typedef uint64_t hrt_abstime;

extern uint64_t _time_now_us;
static inline hrt_abstime hrt_absolute_time() {
    return _time_now_us;
}
