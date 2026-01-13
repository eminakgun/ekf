cmake_minimum_required(VERSION 3.10)
project(standalone_ekf)

project(standalone_ekf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Os -ffunction-sections -fdata-sections)
add_compile_options(-march=rv64imafd -mabi=lp64d -mcmodel=medany)
add_compile_options(-specs=htif_nano.specs)
add_link_options(-Wl,--gc-sections)
add_link_options(-static)
add_link_options(-specs=htif_nano.specs)
# Use htif linker script from Chipyard tests
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/../../tests/htif.ld")

# Include directories
include_directories(
    src
    src/ekf
    src/ekf/python
    src/lib
    src/lib/matrix
    src/lib/mathlib
    src/lib/geo
    src/lib/lat_lon_alt
    src/lib/atmosphere
    src/lib/world_magnetic_model
    src/platform
    include
)

# Define sources
file(GLOB_RECURSE EKF_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/ekf/*.cpp")
file(GLOB_RECURSE MATRIX_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/lib/matrix/*.cpp")
file(GLOB_RECURSE MATHLIB_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/lib/mathlib/*.cpp")
file(GLOB_RECURSE GEO_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/lib/geo/*.cpp")
file(GLOB_RECURSE LATLON_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/lib/lat_lon_alt/*.cpp")
# Atmosphere is header only usually, but let's check
file(GLOB_RECURSE ATMOSPHERE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/lib/atmosphere/*.cpp")
# file(GLOB_RECURSE WMM_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/lib/world_magnetic_model/*.cpp")

# Remove some files that might be problematic or unnecessary
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*python.*")
list(FILTER WMM_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER MATRIX_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER MATRIX_SOURCES EXCLUDE REGEX ".*Test.*")
list(FILTER MATHLIB_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER MATHLIB_SOURCES EXCLUDE REGEX ".*Test.*")
list(FILTER GEO_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER LATLON_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER ATMOSPHERE_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*airspeed.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*drag.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*optical_flow.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*external_vision.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*aux_global_position.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*gravity_fusion.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*auxvel.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*terrain.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*gnss_yaw.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*wind.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*sideslip.*")
list(FILTER EKF_SOURCES EXCLUDE REGEX ".*range_finder.*")

# Create library
add_library(ekf STATIC
    ${EKF_SOURCES}
    ${MATRIX_SOURCES}
    ${MATHLIB_SOURCES}
    ${GEO_SOURCES}
    ${LATLON_SOURCES}
    ${ATMOSPHERE_SOURCES}
    # ${WMM_SOURCES}
)

# Platform specific definitions
add_definitions(-D__PX4_POSIX)
add_definitions(-DECL_STANDALONE)

# EKF Configs
add_definitions(-DCONFIG_EKF2_GNSS)
add_definitions(-DCONFIG_EKF2_BAROMETER)
add_definitions(-DCONFIG_EKF2_MAGNETOMETER)
# add_definitions(-DCONFIG_EKF2_AIRSPEED)
# add_definitions(-DCONFIG_EKF2_SIDESLIP)
# add_definitions(-DCONFIG_EKF2_DRAG_FUSION)
# add_definitions(-DCONFIG_EKF2_RANGE_FINDER)
# add_definitions(-DCONFIG_EKF2_EXTERNAL_VISION)
# add_definitions(-DCONFIG_EKF2_AUXVEL)
# add_definitions(-DCONFIG_EKF2_OPTICAL_FLOW)
# add_definitions(-DCONFIG_EKF2_TERRAIN)
# add_definitions(-DCONFIG_EKF2_WIND)
# add_definitions(-DCONFIG_EKF2_GNSS_YAW)
# add_definitions(-DCONFIG_EKF2_GRAVITY_FUSION)
# add_definitions(-DCONFIG_EKF2_AUX_GLOBAL_POSITION)
add_definitions(-DMODULE_NAME="ekf2") # To satisfy some checks if any

# Demo App
add_executable(demo_app main.cpp)
target_link_libraries(demo_app ekf)
