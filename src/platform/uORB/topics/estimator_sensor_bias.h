#pragma once
#include <stdint.h>

struct estimator_sensor_bias_s {
    uint64_t timestamp;
    uint64_t timestamp_sample;
    // Add generic fields that might be used
    float data[32];
    bool valid;
};
