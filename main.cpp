#include "src/ekf/ekf.h"
#include <cstdio>
#include <vector>

#include <cmath>

// Mock time
uint64_t _time_now_us = 0;
extern uint64_t _time_now_us;

// Helper to check convergence
void check_convergence(const char* test_name, float actual, float expected, float tolerance) {
    if (std::fabs(actual - expected) < tolerance) {
        printf("[PASS] %s: %.4f (Expected %.4f)\n", test_name, actual, expected);
    } else {
        printf("[FAIL] %s: %.4f (Expected %.4f)\n", test_name, actual, expected);
    }
}

int main(int argc, char *argv[])
{
    Ekf ekf;
    setbuf(stdout, NULL);
    _time_now_us = 1000000; // Start at 1s
    ekf.init(_time_now_us);

    printf("EKF Initialized\n");

    // Simulate shorter duration for Verilator (e.g., 2 seconds total)
    // --------------------------------------------------------------------------
    // Test 1: Static Convergence (1 second)
    // --------------------------------------------------------------------------
    printf("\n--- Test 1: Static Convergence (1s) ---\n");
    const float dt = 0.01f; // 100Hz
    int steps = 10; // 0.1 second - minimal for fast RTL sim


    for (int i = 0; i < steps; ++i) {
        _time_now_us += (uint64_t)(dt * 1e6);

        imuSample imu;
        imu.time_us = _time_now_us;
        imu.delta_ang_dt = dt;
        imu.delta_vel_dt = dt;
        imu.delta_ang = matrix::Vector3f(0.0f, 0.0f, 0.0f);
        imu.delta_vel = matrix::Vector3f(0.0f, 0.0f, -9.80665f * dt); // Gravity

        ekf.setIMUData(imu);

        if (i % 10 == 0) { // 10Hz GPS/Baro
            gnssSample gps;
            gps.time_us = _time_now_us;
            gps.lat = 47.378301;
            gps.lon = 8.538777;
            gps.alt = 400.0f;
            gps.fix_type = 3;
            gps.nsats = 10;
            gps.pdop = 1.0f;
            gps.hacc = 0.5f;
            gps.vacc = 0.5f;
            gps.sacc = 0.1f;
            gps.yaw = NAN;
            ekf.setGpsData(gps);

            baroSample baro;
            baro.time_us = _time_now_us;
            baro.hgt = 400.0f;
            ekf.setBaroData(baro);

            magSample mag;
            mag.time_us = _time_now_us;
            mag.mag = matrix::Vector3f(0.2f, 0.0f, 0.4f);
            ekf.setMagData(mag);
        }

        ekf.update();
    }

    // Verify Static State
    matrix::Vector3f vel = ekf.getVelocity();
    check_convergence("Static Vel N", vel(0), 0.0f, 0.1f);
    check_convergence("Static Vel E", vel(1), 0.0f, 0.1f);
    check_convergence("Static Vel D", vel(2), 0.0f, 0.1f);

    matrix::Vector3f pos = ekf.getPosition();
    check_convergence("Static Pos N", pos(0), 0.0f, 1.0f); // Local NED should be near 0 relative to origin


    // --------------------------------------------------------------------------
    // Test 2: Moving East at 5 m/s (10 seconds)
    // --------------------------------------------------------------------------
    printf("\n--- Test 2: Constant Velocity East 5 m/s (10s) ---\n");

    ekf.set_in_air_status(true);
    ekf.set_vehicle_at_rest(false);

    // Start moving
    for (int i = 0; i < steps; ++i) {
        _time_now_us += (uint64_t)(dt * 1e6);
        float t = i * dt;

        imuSample imu;
        imu.time_us = _time_now_us;
        imu.delta_ang_dt = dt;
        imu.delta_vel_dt = dt;
        imu.delta_ang = matrix::Vector3f(0.0f, 0.0f, 0.0f);
        // No specific force in X/Y when moving constant velocity (flat earth).
        // Only gravity in Z.
        imu.delta_vel = matrix::Vector3f(0.0f, 0.0f, -9.80665f * dt);

        ekf.setIMUData(imu);

        if (i % 10 == 0) { // 10Hz GPS
             // Simulate GPS moving East
            gnssSample gps;
            gps.time_us = _time_now_us;
            gps.lat = 47.378301; // Constant Lat

            // 1 degree longitude ~= 75000m at 47 degrees latitude
            // delta_lon = (vel * t) / meters_per_deg_lon
            double dist_east = 5.0 * t; // 5 m/s
            gps.lon = 8.538777 + (dist_east / 75000.0);

            gps.alt = 400.0f;
            gps.fix_type = 3;
            gps.nsats = 10;
            gps.pdop = 1.0f;
            gps.hacc = 0.5f;
            gps.vacc = 0.5f;
            gps.sacc = 0.1f;

            // GPS Velocity
            gps.vel = matrix::Vector3f(0.0f, 5.0f, 0.0f);
            gps.sacc = 0.2f;

            ekf.setGpsData(gps);

            baroSample baro;
            baro.time_us = _time_now_us;
            baro.hgt = 400.0f;
            ekf.setBaroData(baro);

            if (i % 100 == 0) {
                 ekf.print_status();
                 printf("Control Flags (Air: %d, Rest: %d, GNSS: %lu, GNSS Vel: %lu)\n",
                    ekf.get_in_air_status(),
                    ekf.control_status_flags().vehicle_at_rest,
                    ekf.control_status_flags().gnss_pos,
                    ekf.control_status_flags().gnss_vel
                    );
            }
        }

        ekf.update();
    }

    vel = ekf.getVelocity();
    check_convergence("Moving Vel N", vel(0), 0.0f, 1.0f);
    check_convergence("Moving Vel E", vel(1), 5.0f, 1.0f);

    return 0;
}
