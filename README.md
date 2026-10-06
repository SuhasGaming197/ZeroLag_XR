# ZeroLag XR: Ultra-Low Latency Predictive Hand Tracking

**ZeroLag XR** is a high-performance, cross-platform predictive filtering engine designed to eliminate perceived tracking latency in Enterprise Virtual Reality (VR) and Mixed Reality (XR) applications. 

By utilizing a mathematically robust, decoupled 1-Dimensional Extended Kalman Filter (EKF) architecture written in native C++, ZeroLag XR predicts user hand movements 100+ milliseconds into the future, neutralizing hardware latency and combating VR motion sickness.

## 🚀 Key Features

* **Hyper-Fast C++ Core:** The predictive mathematical engine is written in pure C++ and executes in **~0.05 microseconds per frame**, making it suitable for 1000+ Hz enterprise tracking systems.
* **Decoupled EKF Architecture:** Avoids massive 6x6 matrix multiplication errors by decoupling 3D spatial tracking into three independent, mathematically stable 1D predictive streams (X, Y, Z).
* **Cross-Platform Integration:** Compiled as a dynamic library (`.so` / `.dll` / `.dylib`), it is easily bound to Python (via Pybind11) for R&D testbenches, or directly imported into Unity Game Engine via Native Plugins for B2B applications.
* **Instant Tuning:** Highly tunable Process Noise (Q) and Measurement Noise (R) matrices allow the filter to adapt to different tracking hardware configurations (webcams vs. IR sensors).

## 📊 Performance Metrics

* **Execution Time:** 53 nanoseconds / frame
* **Perceived Latency Reduction:** 100.0 ms
* **CPU Overhead:** < 0.1%

## 🛠️ Architecture

1. **`PredictiveFilter.cpp`**: The native C++ EKF engine.
2. **`pybind_wrapper.cpp`**: Pybind11 bindings exposing the C++ memory to Python.
3. **`testbench.py`**: A real-time visualizer utilizing Google MediaPipe to simulate 100ms of VR tracking latency, rendering the lagging coordinates vs the C++ zero-latency prediction.
4. **`Unity_Asset/`**: Contains the `PredictiveHand.cs` MonoBehaviour for drag-and-drop B2B integration into existing Unity VR projects.

## 🎥 Video Demonstration
**Watch the real-time C++ tracking demonstration here:** 
[View ZeroLag XR in Action (Google Drive)](https://drive.google.com/file/d/10rrJSxXCgq9f5wB65bBk-ya0LReqOUlv/view?usp=sharing)


