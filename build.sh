#!/bin/bash
set -e

# Source the Chipyard environment
# Assuming this script is located in software/ekf/
# and env.sh is in the root of the workspace (../../env.sh)
source ../../env.sh

# Create build directory
mkdir -p build
cd build

# Run CMake with the RISC-V toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=../riscv_toolchain.cmake -DCONFIG_EKF2_GNSS=ON -DCONFIG_EKF2_BAROMETER=ON -DCONFIG_EKF2_MAGNETOMETER=ON

# Build
make -j$(nproc)
