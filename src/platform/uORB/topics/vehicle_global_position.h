#pragma once
#include <stdint.h>

struct vehicle_global_position_s {
    uint64_t timestamp;
    uint64_t timestamp_sample;
    double lat;
    double lon;
    float alt;
    float alt_ellipsoid;
    float delta_alt;
    float eph;
    float epv;
    float terrain_alt;
    bool terrain_alt_valid;
    bool dead_reckoning;
    uint8_t lat_lon_reset_counter;
    uint8_t alt_reset_counter;
};
