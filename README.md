# Standalone EKF Library

This project is a standalone port of the PX4-Autopilot EKF2 (Extended Kalman Filter), designed for deployment on RISC-V processors (and other platforms) without the full PX4 dependency stack.

## Features

This build of the EKF is optimized for a restricted hardware environment (50MHz RISC-V) and includes the following features:

### Enabled Sources
*   **IMU Fusion**: Core state prediction using Gyroscope and Accelerometer.
*   **GNSS Fusion**: 3D Position and Velocity fusion.
*   **Barometer Fusion**: Altitude fallback.
*   **Magnetometer Fusion**: Heading fusion.

### Disabled Configuration (Feature Stripping)
To save code size and complexity, the following features are **disabled** at compile time:
*   Airspeed Fusion (Fixed Wing)
*   Sideslip Fusion
*   Drag Fusion (Multirotor wind estimation)
*   Range Finder Fusion
*   Optical Flow
*   External Vision (VIO/SLAM)
*   Auxiliary Global Position
*   Terrain Estimation
*   Wind Estimation

## Build System

The project uses CMake and supports cross-compilation.

### Building for Host (Linux)
```bash
cd build
cmake ..
make
./demo_app
```

### Building for RISC-V (RV64GC)
The project includes a toolchain file for `riscv64-linux-gnu-g++`.

```bash
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../riscv_toolchain.cmake ..
make
```

## API Usage

The core class is `Ekf` (in `src/ekf/ekf.h`).

### 1. Initialization
```cpp
#include "src/ekf/ekf.h"

Ekf ekf;
uint64_t now_us = 1000000; // Time in microseconds
ekf.init(now_us);
```

### 2. Parameter Configuration
Access the parameter struct directly via `getParamHandle()`. Parameters are defined in `src/platform/px4_platform_common/module_params.h`.

```cpp
parameters *params = ekf.getParamHandle();

// Example: Set GPS position noise
params->ekf2_gps_p_noise = 0.5f;

// Example: Set Gyro noise
params->ekf2_gyr_noise = 1.5e-2f;

ekf.updateParameters(); // Commit changes (if needed by internal logic)
```

**Key Parameters (Partial List):**
*   `ekf2_gyr_noise`: Gyro noise density (rad/s / sqrt(Hz))
*   `ekf2_acc_noise`: Accel noise density (m/s^2 / sqrt(Hz))
*   `ekf2_gps_p_noise`: GPS horizontal position noise (m)
*   `ekf2_gps_v_noise`: GPS velocity noise (m/s)
*   `ekf2_baro_noise`: Barometer noise (m)
*   `ekf2_mag_noise`: Magnetometer noise (Gauss)

### 3. Feeding Data
The EKF pulls data from internal buffers. You must push data into these buffers.

**IMU (High Rate - Main Prediction):**
```cpp
imuSample imu;
imu.time_us = now_us;
imu.delta_ang_dt = dt_sec;
imu.delta_vel_dt = dt_sec;
imu.delta_ang = Vector3f(gyr_x, gyr_y, gyr_z) * dt_sec;
imu.delta_vel = Vector3f(acc_x, acc_y, acc_z) * dt_sec;

ekf.setIMUData(imu);
```

**GNSS (Lower Rate):**
```cpp
gnssSample gps;
gps.time_us = now_us;
gps.lat = 47.378301; // Degrees
gps.lon = 8.538777;
gps.alt = 400.0f;    // Meters (AMSL)
gps.fix_type = 3;    // 3D Fix
gps.sacc = 0.5f;     // Speed accuracy (m/s)
gps.hacc = 0.8f;     // Horiz accuracy (m)
gps.vacc = 1.0f;     // Vert accuracy (m)

ekf.setGpsData(gps);
```

**Barometer (Lower Rate):**
```cpp
baroSample baro;
baro.time_us = now_us;
baro.hgt = 405.0f; // Meters (Pressure altitude)

ekf.setBaroData(baro);
```

### 4. Updating and Getting Output
Call `update()` after setting IMU data. It returns `true` if a state update was performed (after downsampling).

```cpp
if (ekf.update()) {
    // Get Position (Local NED)
    Vector3f pos = ekf.getPosition(); // (North, East, Down) in meters

    // Get Velocity (Local NED)
    Vector3f vel = ekf.getVelocity(); // (North, East, Down) in m/s

    // Get Attitude (Quaternion)
    Quatf q = ekf.getQuaternion();
    Eulerf euler(q); // Convert to Roll, Pitch, Yaw

    // Get Global Position (Lat/Lon/Alt)
    LatLonAlt global_pos = ekf.getLatLonAlt();
}
```

## Memory Usage
By default, the system uses **dynamic allocation** (`new`) for RingBuffers during initialization.
*   **Heap Usage**: ~50-60KB (Estimated for default buffer sizes)
*   **Stack Usage**: Moderate (Recursive templates are limited).

## Advanced Usage & FAQ

### 1. What if I don't have a Barometer or Magnetometer?

**No Barometer:**
You can use GNSS (GPS) altitude instead.
*   Set parameter `EKF2_HGT_REF` to `1` (GNSS).
*   Default is `0` (Barometer).

```cpp
parameters *params = ekf.getParamHandle();
// Set height source to GNSS
// Note: You need to implement the mapping for this param in your parameter system
// or manually set the internal logic if you modified ModuleParams.
// In standard PX4: 0=Baro, 1=GPS, 2=Range, 3=EV
params->ekf2_hgt_ref = 1;
```

**No Magnetometer:**
The EKF can operate without a magnetometer, but yaw observability is reduced.
*   **Stationary**: Yaw will drift unless constrained by another source (e.g., External Vision) or manually reset.
*   **Moving**: The EKF will align yaw to the GPS course (velocity vector) once sufficient speed is reached.
*   **Setup**: Simply do not call `setMagData()`. You might want to disable magnetic field strength checks by setting `ekf2_mag_check` to `0` or ensuring `EKF2_MAG_TYPE` is set to "None" (if available in your param logic).

### 2. How to implement `now_us` on RISC-V?

You need a monotonic microsecond source.

**On Linux (RV64GC):**
```cpp
#include <time.h>

uint64_t get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}
```

**On Bare Metal:**
Read the RISC-V `mtime` CSR (Control and Status Register) or a hardware timer tick, and convert to microseconds.

### 3. How to calculate `dt_sec`?

`dt_sec` is the time difference between the **current** IMU sample and the **previous** IMU sample.

```cpp
static uint64_t last_imu_us = 0;
uint64_t now = get_time_us();

if (last_imu_us == 0) {
    last_imu_us = now;
    return; // Skip first sample
}

float dt_sec = (now - last_imu_us) * 1e-6f;
last_imu_us = now;

// Feed dt_sec to EKF
imu.delta_ang_dt = dt_sec;
imu.delta_vel_dt = dt_sec;
```
*Note: Ensure your IMU driver provides consistent rates. Jitter in `dt` can affect integration accuracy.*

### 4. Memory Usage

For the `demo_app` (RV64GC build, `-Os` optimized, WMM removed):

*   **Code (.text)**: ~92 KB
*   **Static Data (.data + .bss)**: ~1.5 KB
*   **Heap (Dynamic)**: ~60 KB (Dependent on `RingBuffer` sizes)
*   **Stack**: ~4-8 KB (Estimate)

Total Runtime Footprint: ~155-160 KB.
To reduce Heap usage, you can revert to **Static Allocation** (requires code modification to `RingBuffer.h` and `estimator_interface.h` as previously explored).

