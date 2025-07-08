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

## Project Features

- Web-based attendance terminal served directly from ESP32
- Buttons for **Entry**, **Exit**, and **Absent** (5 persons)
- Timestamps with **real-time clock (NTPClient)** and status storage on Firebase
- Calculates duration (between entry and exit)
- Prevents multiple entries or exits on the same day
- Attendance data stored securely in **Cloud Firestore**
- Hosted HTML UI served via ESP32
- Clean and responsive HTML UI (no external server required)
- Works completely wirelessly on local WiFi
- Uses Firebase authentication and token refresh

  
---

## How It Works
1. ESP32 connects to WiFi and Firebase using API key.
2. Web interface displays buttons for each person.
3. When a button is pressed (Entry/Exit/Absent), the ESP32:
   - Gets current timestamp via NTP
   - Writes data to Firestore (under a nested map for each person)
4. Data includes status (entry/exit/absent), time, and duration (for exit).

## Web Interface Preview

The ESP32 serves this interface on your local WiFi network. Each person has:
- **Entry** button → logs time and status as "Present"
- **Exit** button → records exit time and total duration
- **Absent** button → marks absent without entry

*NOTE*: Once a button is pressed, others are disabled for the day to prevent duplicate entries.

---
## How to Set Up

1. **Connect ESP32** to Wi-Fi:
   ```
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";

2. **Configure Firebase**

   ```
   #define API_KEY "your_api_key"  
   #define FIREBASE_PROJECT_ID "your_project_id"  
   #define USER_EMAIL "your_firebase_email"  
   #define USER_PASSWORD "your_firebase_password"

  
3. Upload the code using PlatformIO or Arduino IDE.

&nbsp;&nbsp;&nbsp;&nbsp;**ESP32 will print its IP in Serial Monitor. Open it in browser:** 
&nbsp;&nbsp;`http://<ESP32-IP>`

## Firestore Data Structure

**/Attendance/**  
&nbsp;&nbsp;&nbsp;**/Person1/**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`{`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`status: "entry"`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`time: "2025-07-07 10:30:00"`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`duration: "2h 45m" (only for exit)`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`}`

&nbsp;&nbsp;**/Person2/**  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`{`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`status: "entry"`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`time: "2025-07-07 10:00:00"`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`duration: "3h 30m" (only for exit)`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`}`

---

##  Future Improvements
- Add biometric sensor (Fingerprint/RFID)
- User authentication
- Firebase Cloud Functions for alerts


<h2> View Project Visuals </h2>

<ul>
  <li><a href="Images/UI_Web_Interface.png" target="_blank">➡ View web UI Interface with improved layout</a></li>
  <li><a href="Images/data_flow.png" target="_blank">➡ View Block Diagram of Data Flow to Firestore</a></li>
  <li><a href="Images/Cloud_Firestore_data_storage.png" target="_blank">➡ View Cloud Firestore - Data Storage</a></li>
</ul>







