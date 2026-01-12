#pragma once
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <drivers/drv_hrt.h>

#ifndef M_PI_F
#define M_PI_F 3.14159265358979323846f
#endif
#ifndef M_TWOPI_F
#define M_TWOPI_F (2.0f * M_PI_F)
#endif
#ifndef M_PI_2_F
#define M_PI_2_F (M_PI_F / 2.0f)
#endif

#define PX4_ISFINITE(x) std::isfinite(x)
#define ECL_INFO(...)
#define ECL_WARN(...)
#define ECL_ERR(...)
#define ECL_DEBUG(...)

// Mock for parameters
struct Ekf2Params {
    float ekf2_gyr_noise = 1.5e-2f;
    float ekf2_acc_noise = 3.5e-1f;
    float ekf2_gyr_b_noise = 1.0e-3f;
    float ekf2_acc_b_noise = 3.0e-3f;
    float ekf2_mag_e_noise = 1.0e-1f;
    float ekf2_mag_b_noise = 1.0e-4f;
    float ekf2_wind_nsd = 1.0e-1f;
    float ekf2_head_noise = 3.0e-1f;
    float ekf2_mag_decl = 0.0f;
    float ekf2_mag_gate = 3.0f;
    float ekf2_baro_gate = 5.0f;
    float ekf2_gps_p_gate = 5.0f;
    float ekf2_gps_v_gate = 5.0f;
    float ekf2_tas_gate = 3.0f;
    float ekf2_hgt_a_gate = 5.0f; // Height anomaly gate
    float ekf2_drag_noise = 2.5f;
    float ekf2_of_n_min = 0.15f;
    float ekf2_of_n_max = 0.5f;
    float ekf2_of_gate = 3.0f;

    // Limits
    float ekf2_gyr_b_lim = 0.15f; // 8.5 deg/s
    float ekf2_abl_lim = 0.4f;
    float ekf2_vel_lim = 100.0f; // m/s

    // Delays
    float ekf2_predict_us = 10000.0f; // 10ms

    // Other
    float ekf2_min_rng = 0.1f;
    float ekf2_rng_pitch = 0.0f;
    float range_cos_max_tilt = 0.707f;
    float ekf2_rng_qlty_t = 1.0f;
    float ekf2_rng_fog = 0.0f;

    int32_t ekf2_imu_ctrl = 0; // Gyro bias control
};

// Mock for perf counters
typedef void* perf_counter_t;
static inline perf_counter_t perf_alloc(int, const char*) { return nullptr; }
static inline void perf_begin(perf_counter_t) {}
static inline void perf_end(perf_counter_t) {}
static inline void perf_free(perf_counter_t) {}
static inline void perf_count(perf_counter_t) {}
static inline void perf_set_elapsed(perf_counter_t, int64_t) {}

enum PC_TYPE {
    PC_ELAPSED,
    PC_COUNT,
    PC_INTERVAL
};

