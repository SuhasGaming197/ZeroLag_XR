#!/bin/bash
echo "Installing dependencies..."
python3 -m pip install opencv-python mediapipe numpy pybind11

echo "Compiling C++ Pybind11 module..."
# Compile using pybind11
c++ -O3 -Wall -shared -std=c++14 -fPIC $(python3 -m pybind11 --includes) pybind_wrapper.cpp PredictiveFilter.cpp -o zerolag$(python3-config --extension-suffix) -undefined dynamic_lookup

echo "Compilation successful!"
echo "Run 'python3 testbench.py' to start the ZeroLag XR visualizer."
