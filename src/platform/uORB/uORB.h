#pragma once
#include <stdint.h>

typedef const struct orb_metadata *orb_id_t;

struct orb_metadata {
    const char *o_name;
    uint16_t o_size;
    uint16_t o_size_no_padding;
    const char *o_fields;
};

#define ORB_ID(topic) ((orb_id_t)0)
#define ORB_DECLARE(topic) extern const struct orb_metadata __orb_##topic
