#pragma once
#include <stdint.h>

class ModuleParams {
public:
    ModuleParams(void *parent) {}
    virtual ~ModuleParams() = default;
    void updateParams() {}
};

template <int32_t P>
class ParamInt {
public:
    int32_t get() const { return 0; }
};

template <int32_t P>
class ParamFloat {
public:
    float get() const { return 0.0f; }
};

namespace px4 {
namespace params {
    enum {
        EKF2_AGP_CTRL,
        EKF2_AGP_MODE,
        EKF2_AGP_DELAY,
        EKF2_AGP_NOISE,
        EKF2_AGP_GATE,
        // Add others as needed
        EKF2_PREDICT_US,
        EKF2_MAG_DELAY,
        EKF2_MAG_TYPE,
        EKF2_MAG_ACCLIM,
        EKF2_MAG_YAWLIM,
        EKF2_MAG_DECL,
        EKF2_MAG_CHECK,
        EKF2_MAG_E_NOISE,
        EKF2_MAG_B_NOISE,
        EKF2_MAG_NOISE,
        EKF2_MAG_GATE,
        EKF2_HGT_REF,
        EKF2_NOAID_NOISE,
        EKF2_GPS_CTRL,
        EKF2_GPS_DELAY,
        EKF2_GPS_HGT_BIAS,
        EKF2_GPS_V_NOISE,
        EKF2_GPS_P_NOISE,
        EKF2_GPS_P_GATE,
        EKF2_GPS_V_GATE,
        EKF2_GPS_CHECK,
        EKF2_REQ_EPH,
        EKF2_REQ_EPV,
        EKF2_REQ_SACC,
        EKF2_REQ_NSATS,
        EKF2_REQ_PDOP,
        EKF2_REQ_HDRIFT,
        EKF2_REQ_VDRIFT,
        EKF2_REQ_GPS_H,
        EKF2_ASP_DELAY,
        EKF2_TAS_GATE,
        EKF2_EAS_NOISE,
        EKF2_ARSP_THR,
        EKF2_DRAG_NOISE,
        EKF2_BCOEF_X,
        EKF2_BCOEF_Y,
        EKF2_MCOEF,
        EKF2_OF_DELAY,
        EKF2_OF_N_MIN,
        EKF2_OF_N_MAX,
        EKF2_OF_QMIN,
        EKF2_OF_GATE,
        EKF2_TERR_NOISE,
        EKF2_TERR_GRAD,
        EKF2_RNG_CTRL,
        EKF2_RNG_DELAY,
        EKF2_RNG_NOISE,
        EKF2_RNG_GATE,
        EKF2_RNG_PITCH,
        EKF2_RNG_AID,
        EKF2_RNG_A_VMAX,
        EKF2_RNG_A_HMAX,
        EKF2_RNG_SFE,
        EKF2_RNG_K_GATE,
        EKF2_RNG_QLTY_T,
        EKF2_RNG_FOG,
        EKF2_EV_DELAY,
        EKF2_EV_QMIN,
        EKF2_EVP_NOISE,
        EKF2_EVV_NOISE,
        EKF2_EVA_NOISE,
        EKF2_EVP_GATE,
        EKF2_EVV_GATE,
        EKF2_EV_NOISE_MD,
        EKF2_AVEL_DELAY,
        EKF2_GYR_NOISE,
        EKF2_ACC_NOISE,
        EKF2_GYR_B_NOISE,
        EKF2_ACC_B_NOISE,
        EKF2_HEAD_NOISE,

        EKF2_GBIAS_INIT,
        EKF2_ABIAS_INIT,
        EKF2_ANGERR_INIT,
        EKF2_ABL_LIM,
        EKF2_ABL_ACCLIM,
        EKF2_ABL_GYRLIM,
        EKF2_ABL_TAU,
        EKF2_GYR_B_LIM,
        EKF2_PD_H,
        EKF2_PD_V,
        EKF2_PD_P,
        EKF2_PD_I,
        EKF2_PD_D,
        EKF2_PD_F,
        EKF2_PD_MAX,
        EKF2_GSF_TAS,
        EKF2_BARO_CTRL,
        EKF2_BARO_DELAY,
        EKF2_BARO_NOISE,
        EKF2_BARO_GATE,
        EKF2_GND_EFF_DZ,
        EKF2_GND_MAX_HGT,
        EKF2_PCOEF_XP,
        EKF2_PCOEF_XN,
        EKF2_PCOEF_YP,
        EKF2_PCOEF_YN,
        EKF2_PCOEF_Z,
        EKF2_ASP_MAX,
        EKF2_WIND_NSD,
    };
}
}
