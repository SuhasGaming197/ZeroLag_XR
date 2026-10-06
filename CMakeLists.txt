cmake_minimum_required(VERSION 3.10)
project(ZeroLag_XR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")

# Compile the Benchmark test executable
add_executable(RunBenchmark benchmark.cpp PredictiveFilter.cpp)
