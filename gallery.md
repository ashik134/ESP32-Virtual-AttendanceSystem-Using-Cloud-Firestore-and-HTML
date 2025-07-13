# ESP32 Virtual Attendance System Using Cloud Firestore and HTML UI

This is a cloud-connected attendance system built using **ESP32**, **Firebase Firestore**, and **PlatformIO (VS Code)**. It allows tracking of daily **entry**, **exit**, and **absence** status for up to 5 individuals via a custom web interface hosted on the ESP32.
This project implements a **Wi-Fi based virtual attendance system** using an **ESP32**, a **responsive web interface**, and **Firebase Cloud Firestore** to store and manage attendance data in real time.

---

## Tech Stack

- **Board**: ESP32 DevKit v1 (Core microcontroller)
- **Platform**: PlatformIO (VS Code)
- **Language**: C/C++
- **Cloud Backend**: Firebase Firestore (NoSQL database)
- **Network**: WiFi (NTPClient for time sync)
- **Web Interface**: HTML + CSS + JS - Web UI served from ESP32 memory
- **Firebase Auth**: User authentication
-  WebServer.h   - Local server on ESP32               
-  NTPClient     - Get current time via internet       
-  WiFi.h        - WiFi connectivity                   

---

## How It Works
1. ESP32 connects to WiFi and Firebase using API key.
2. Web interface displays buttons for each person.
3. When a button is pressed (Entry/Exit/Absent), the ESP32:
   - Gets current timestamp via NTP
   - Writes data to Firestore (under a nested map for each person)
4. Data includes status (entry/exit/absent), time, and duration (for exit).

<h2> View Project Visuals </h2>

<ul>
  <li><a href="Images/UI_Web_Interface.png" target="_blank">➡ View web UI Interface with improved layout</a></li>
  <li><a href="Images/data_flow.png" target="_blank">➡ View Block Diagram of Data Flow to Firestore</a></li>
  <li><a href="Images/Cloud_Firestore_data_storage.png" target="_blank">➡ View Cloud Firestore - Data Storage</a></li>
</ul>
