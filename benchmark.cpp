#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>

extern "C" {
    void InitFilter();
    void UpdateFilter(float x, float y, float z, float dt);
    void PredictFuture(float forward_time_ms, float* out_x, float* out_y, float* out_z);
}

int main() {
    InitFilter();
    
    const int num_frames = 100000;
    const float dt = 0.00833f; // 120Hz refresh rate (1/120)
    
    // Warmup
    UpdateFilter(0, 0, 0, dt);

    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_frames; ++i) {
        float t = i * dt;
        float x = std::sin(t);
        float y = std::cos(t);
        float z = std::sin(t * 0.5f);
        
        UpdateFilter(x, y, z, dt);
        
        float px, py, pz;
        PredictFuture(100.0f, &px, &py, &pz); // Predict 100ms into the future
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    
    double total_ms = elapsed.count() / 1000.0;
    double avg_us = elapsed.count() / num_frames;
    
    std::cout << "========================================\n";
    std::cout << "⚡ ZERO-LAG KINEMATICS PERFORMANCE TEST ⚡\n";
    std::cout << "========================================\n";
    std::cout << "Frames Simulated:  " << num_frames << "\n";
    std::cout << "Target Framerate:  120 Hz\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total CPU Time:    " << total_ms << " ms\n";
    std::cout << std::setprecision(3);
    std::cout << "Avg Time / Frame:  " << avg_us << " µs\n";
    std::cout << "Status:            " << (avg_us < 100.0 ? "PASS (Well under 100µs limit)" : "FAIL") << "\n";
    std::cout << "========================================\n";
    
    return 0;
}
