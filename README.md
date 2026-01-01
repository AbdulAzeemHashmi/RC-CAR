## RC-CAR
Implementation of Tilt Handling and Robust Fault Management for an Advanced Radar Surveillance System using ESP32, C++, and Python.

## Advanced Radar Surveillance System - Reliability & Fault Management Layer
### 📌 Project Overview
This project features an ESP32-based autonomous radar surveillance rover designed for fixed-route monitoring in environments like warehouses and restricted corridors. The system integrates multi-sensor fusion (Ultrasonic, IR, LDR, and Gyroscope) to provide real-time environmental mapping via a Python-based dashboard.

### 🛠️ Individual Contribution: Abdul Azeem (24I-2013)
In this collaborative project, my primary focus was on ensuring the operational reliability and environmental stability of the rover through advanced logic implementation in C++ (Arduino).

### Key Features Implemented:
#### 1. Tilt & Uneven Surface Handling (Feature 7)

Instability Detection: Developed logic using gyroscope/tilt sensors to detect directional instability or uneven terrain.

System Safety Lock: When a tilt is detected (systemTilted), the rover enters a "Locked" state, and the sensor-scanning servo is automatically centered to 90 degrees to prevent hardware damage.

Alert Feedback: Implemented a high-priority alert system where the LED blinks at 200ms intervals and the buzzer emits a 1500Hz tone during instability.

#### 2. Robust Fault Management (Feature 8)
Sensor Reliability Monitoring: Created a fault detection window (10 seconds) to monitor PIR/Motion sensors. If sensors become unresponsive, the system logs a fault and triggers an indicator.

Anti-Stuck Logic: Implemented a watchdog-style check that monitors the tilt sensor. If the sensor remains in the same state for more than 30 seconds, a "Stuck Sensor" warning is issued via the Serial Monitor.

Error Indicators: Developed the faultIndicator() function to provide distinct audio-visual feedback when hardware malfunctions are detected.

### 🖥️ Simulation
The hardware design and logic verification were performed on Tinkercad.

Simulation Link: Circuit design Mighty Habbi-Juttuli - Tinkercad 

### 📂 Technical Stack

Microcontroller: ESP32 

Firmware Language: C++ (Arduino IDE), Python

Sensors: MPU6050 (Gyroscope/Tilt), PIR Motion Sensors, Ultrasonic Sensors 

Actuators: Servo Motor (Scanning mechanism)
