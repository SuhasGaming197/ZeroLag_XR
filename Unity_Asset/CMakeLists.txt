cmake_minimum_required(VERSION 3.10)
project(ZeroLagKinematics)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3 -O2")

# Configure library output
add_library(ZeroLagKinematics SHARED PredictiveFilter.cpp)

# Cross-platform notes:
# To compile for Android: Use Android NDK CMake toolchain (-DCMAKE_TOOLCHAIN_FILE=...)
# To compile for Windows: CMake generates .dll automatically
