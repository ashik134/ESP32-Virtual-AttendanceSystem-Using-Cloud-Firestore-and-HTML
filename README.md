# ESP32-Virtual-Attendance-System-Using-Cloud-Firestore-and-HTML

This is a cloud-connected attendance system built using **ESP32**, **Firebase Firestore**, and **PlatformIO (VS Code)**. It allows tracking of daily **entry**, **exit**, and **absence** status for up to 5 individuals via a custom web interface hosted on the ESP32.

---

## Tech Stack

- **Board**: ESP32 DevKit v1
- **Platform**: PlatformIO (VS Code)
- **Language**: C/C++
- **Cloud Backend**: Firebase Firestore (NoSQL)
- **Network**: WiFi (NTPClient for time sync)
- **Web Interface**: HTML hosted by ESP32

---

## Project Features

- Entry, Exit, and Absent buttons for 5 individuals
- Real-time timestamp and status storage on Firebase
- Calculates duration (between entry and exit)
- Prevents multiple entries or exits on the same day
- Hosted HTML UI served via ESP32
- Works completely wirelessly on local WiFi

---

