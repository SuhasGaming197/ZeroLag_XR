using UnityEngine;
using System.Runtime.InteropServices;

/// <summary>
/// PredictiveHand - ZeroLag Kinematics Filter for Unity
/// 
/// INDIE DEVELOPER INSTRUCTIONS:
/// 1. Drop the compiled ZeroLagKinematics.dll (Windows) or .so (Android) into your Unity project's Assets/Plugins folder.
/// 2. Attach this PredictiveHand.cs script to the GameObject you want to apply zero-lag prediction to (e.g., Hand Tracking Model).
/// 3. Assign the target Transform (the raw, laggy hand tracking data) to the 'laggySource' field in the inspector.
/// 4. Adjust the 'predictionTimeMs' to match your VR headset's average lag (default is 100ms).
/// </summary>
public class PredictiveHand : MonoBehaviour
{
    [Tooltip("The raw, delayed transform from your VR SDK (e.g., Oculus Hand Tracking).")]
    public Transform laggySource;

    [Tooltip("How far into the future to predict (in milliseconds). Set this to match your hardware latency.")]
    public float predictionTimeMs = 100.0f;

    const string DLL_NAME = "ZeroLagKinematics";

    [DllImport(DLL_NAME)]
    private static extern void InitFilter();

    [DllImport(DLL_NAME)]
    private static extern void UpdateFilter(float x, float y, float z, float dt);

    [DllImport(DLL_NAME)]
    private static extern void PredictFuture(float forward_time_ms, out float out_x, out float out_y, out float out_z);

    void Start()
    {
        InitFilter();
    }

    void Update()
    {
        if (laggySource == null) return;

        // Feed current laggy data to the C++ Engine
        Vector3 pos = laggySource.position;
        UpdateFilter(pos.x, pos.y, pos.z, Time.deltaTime);

        // Get predicted future position
        float px, py, pz;
        PredictFuture(predictionTimeMs, out px, out py, out pz);

        // Apply to this transform to achieve zero-lag feel
        transform.position = new Vector3(px, py, pz);
    }
}
