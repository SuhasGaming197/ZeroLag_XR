import cv2
import mediapipe as mp
import time
import collections
import numpy as np
import math
import csv

try:
    import zerolag
    C_FILTER_READY = True
    print("ZeroLag C++ module loaded successfully.")
    zerolag.init_filter()
except ImportError:
    C_FILTER_READY = False
    print("WARNING: zerolag module not found. Did you run compile_test.sh? C++ Prediction is disabled.")

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

print("Attempting to open webcam (0)...")
cap = cv2.VideoCapture(0)

for _ in range(5):
    success, img = cap.read()
    if success:
        break
    print("Waiting for camera... (Did you click Allow for permissions?)")
    time.sleep(1)
    cap = cv2.VideoCapture(0)

if not cap.isOpened() or not success:
    print("ERROR: Could not open webcam. macOS might be blocking Terminal in System Settings > Privacy & Security > Camera.")
    exit(1)

DELAY_MS = 100
FPS_GUESS = 30
FRAME_DELAY = max(1, int((DELAY_MS / 1000.0) * FPS_GUESS))

history = collections.deque(maxlen=FRAME_DELAY)
last_time = time.time()

cv2.namedWindow("ZeroLag XR Testbench", cv2.WINDOW_NORMAL)
print("Webcam successfully opened! Look for the video window.")

# Setup CSV Logging to save tracking data for analysis
csv_filename = "kalman_tracking_data.csv"
csv_file = open(csv_filename, "w", newline="")
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["time", "ground_truth_x", "ground_truth_y", "laggy_x", "laggy_y", "predicted_x", "predicted_y", "error_px"])
print(f"Recording prediction data to {csv_filename} ...")

try:
    while True:
        success, img = cap.read()
        if not success:
            time.sleep(0.01)
            continue
            
        current_time = time.time()
        dt = current_time - last_time
        last_time = current_time
        
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        results = hands.process(imgRGB)
        
        h, w, c = img.shape
        ground_truth = None
        
        if results.multi_hand_landmarks:
            for handLms in results.multi_hand_landmarks:
                lm = handLms.landmark[8] # Index finger tip
                cx, cy = int(lm.x * w), int(lm.y * h)
                cz = lm.z * w 
                ground_truth = (cx, cy, cz)
                break
                
        if ground_truth:
            history.append({'time': current_time, 'pos': ground_truth, 'dt': dt})
        
        if len(history) == FRAME_DELAY:
            delayed_data = history.popleft()
            dx, dy, dz = delayed_data['pos']
            d_dt = delayed_data['dt']
            
            # Laggy VR Data (Red Box)
            cv2.rectangle(img, (int(dx)-20, int(dy)-20), (int(dx)+20, int(dy)+20), (0, 0, 255), 2)
            cv2.putText(img, "Laggy VR (100ms)", (int(dx)-50, int(dy)-25), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
            
            if C_FILTER_READY:
                zerolag.update_filter(float(dx), float(dy), float(dz), float(d_dt))
                px, py, pz = zerolag.predict_future(float(DELAY_MS))
                
                # Predicted Future Data (Green Box)
                cv2.rectangle(img, (int(px)-20, int(py)-20), (int(px)+20, int(py)+20), (0, 255, 0), 2)
                cv2.putText(img, "ZeroLag C++", (int(px)-50, int(py)-25), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                
                if ground_truth:
                    gx, gy, gz = ground_truth
                    # Actual Ground Truth (White Dot)
                    cv2.circle(img, (gx, gy), 5, (255, 255, 255), cv2.FILLED)
                    
                    error = math.sqrt((gx - px)**2 + (gy - py)**2)
                    cv2.putText(img, f"Prediction Error: {error:.1f}px", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 0), 2)
                    
                    # Log the data for future modification
                    csv_writer.writerow([current_time, gx, gy, dx, dy, px, py, error])
                    
        cv2.imshow("ZeroLag XR Testbench", img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    cap.release()
    cv2.destroyAllWindows()
    csv_file.close()
    print(f"Data saved successfully to {csv_filename}!")
