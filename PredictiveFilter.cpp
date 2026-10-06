#include <cmath>
#include <iostream>

class Kalman1D {
public:
    float p, v;
    float P[2][2];
    float q_p, q_v, r;
    bool init;

    Kalman1D() {
        p = 0; v = 0;
        P[0][0] = 1000.0f; P[0][1] = 0.0f;
        P[1][0] = 0.0f; P[1][1] = 1000.0f;
        
        // Process noise (Q) - High values allow the filter to instantly adapt to fast wrist flicks
        q_p = 1.0f;    // Position noise
        q_v = 1000.0f; // Velocity noise (extremely high to trust fast acceleration)
        
        // Measurement noise (R) - Low value means we heavily trust the raw webcam pixels
        r = 0.1f;
        init = false;
    }

    void Update(float z, float dt) {
        if (!init) {
            p = z; 
            v = 0;
            init = true;
            return;
        }
        
        if (dt <= 0.0001f) dt = 0.0001f;

        // 1. Predict
        float p_pred = p + v * dt;
        float v_pred = v;

        float P_pred[2][2];
        P_pred[0][0] = P[0][0] + dt * (P[1][0] + P[0][1]) + dt * dt * P[1][1] + q_p;
        P_pred[0][1] = P[0][1] + dt * P[1][1];
        P_pred[1][0] = P[1][0] + dt * P[1][1];
        P_pred[1][1] = P[1][1] + q_v;

        // 2. Update
        float y = z - p_pred; // Error
        float S = P_pred[0][0] + r;
        
        // Kalman Gain
        float K[2];
        K[0] = P_pred[0][0] / S;
        K[1] = P_pred[1][0] / S;

        // Apply Gain
        p = p_pred + K[0] * y;
        v = v_pred + K[1] * y;

        // Update Covariance
        P[0][0] = (1.0f - K[0]) * P_pred[0][0];
        P[0][1] = (1.0f - K[0]) * P_pred[0][1];
        P[1][0] = P_pred[1][0] - K[1] * P_pred[0][0];
        P[1][1] = P_pred[1][1] - K[1] * P_pred[0][1];
    }
};

class PredictiveFilter {
public:
    Kalman1D filterX;
    Kalman1D filterY;
    Kalman1D filterZ;

    PredictiveFilter() {}

    void Update(float mx, float my, float mz, float dt) {
        filterX.Update(mx, dt);
        filterY.Update(my, dt);
        filterZ.Update(mz, dt);
    }

    void PredictFuture(float forward_time_ms, float& out_x, float& out_y, float& out_z) {
        float dt = forward_time_ms / 1000.0f;
        out_x = filterX.p + filterX.v * dt;
        out_y = filterY.p + filterY.v * dt;
        out_z = filterZ.p + filterZ.v * dt;
    }
};

PredictiveFilter global_filter;

#if defined(_WIN32)
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {
    DLL_EXPORT void InitFilter() {
        global_filter = PredictiveFilter();
    }
    
    DLL_EXPORT void UpdateFilter(float x, float y, float z, float dt) {
        global_filter.Update(x, y, z, dt);
    }

    DLL_EXPORT void PredictFuture(float forward_time_ms, float* out_x, float* out_y, float* out_z) {
        global_filter.PredictFuture(forward_time_ms, *out_x, *out_y, *out_z);
    }
}
