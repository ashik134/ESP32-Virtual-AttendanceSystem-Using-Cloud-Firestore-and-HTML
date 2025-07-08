#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Automation_Lab 1";
const char* password = "autoone23";

#define API_KEY "AIzaSyCvhijQq9aRsL5ZLuo4pN0msb0JM8PSdKs"
#define FIREBASE_PROJECT_ID "attendance-webtofirestore"
#define USER_EMAIL "ashikrahim777@gmail.com"
#define USER_PASSWORD "*********"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Web server on port 80
WebServer server(80);

// Store entry times for duration calculation
unsigned long entryTimes[5] = {0};

// Forward declarations
void handleRoot();
void handleEntry();
void handleExit();
void handleAbsent();
String getISOTime();
String getCurrentDate();
String formatDuration(unsigned long ms);
void tokenStatusCallback(TokenInfo info);

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize NTP client
  timeClient.begin();
  timeClient.setTimeOffset(0);

  // Configure Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Set up web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/entry", HTTP_POST, handleEntry);
  server.on("/exit", HTTP_POST, handleExit);
  server.on("/absent", HTTP_POST, handleAbsent);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  timeClient.update();
}

void handleRoot() {
  // Serve the HTML page
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Virtual Attendance Terminal</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
    padding: 20px;
    text-align: center;
    min-height: 100vh;
    margin: 0;
  }
  
  .container {
    max-width: 800px;
    margin: 0 auto;
    padding: 20px;
    background-color: white;
    border-radius: 12px;
    box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
  }
  
  h1 {
    color: #2c3e50;
    margin-bottom: 10px;
  }
  
  .current-time {
    background-color: #3498db;
    color: white;
    padding: 8px 15px;
    border-radius: 20px;
    display: inline-block;
    margin-bottom: 20px;
    font-weight: 500;
  }
  
  table {
    margin: 20px auto;
    border-collapse: collapse;
    width: 100%;
    background-color: white;
    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.08);
    border-radius: 10px;
    overflow: hidden;
  }
  
  th, td {
    padding: 12px 15px;
    border-bottom: 1px solid #ecf0f1;
  }
  
  th {
    background: linear-gradient(to right, #3498db, #2c3e50);
    color: white;
    font-weight: 600;
  }
  
  .btn {
    padding: 10px 18px;
    color: white;
    border: none;
    cursor: pointer;
    margin: 2px;
    border-radius: 6px;
    font-weight: 600;
    transition: all 0.3s ease;
    min-width: 100px;
  }
  
  .btn:hover {
    transform: translateY(-2px);
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  }
  
  .btn:active {
    transform: translateY(0);
  }
  
  .btn-entry { 
    background-color: #2ecc71;
  }
  
  .btn-entry:hover {
    background-color: #27ae60;
  }
  
  .btn-exit { 
    background-color: #e67e22; 
  }
  
  .btn-exit:hover {
    background-color: #d35400;
  }
  
  .btn-absent { 
    background-color: #e74c3c; 
  }
  
  .btn-absent:hover {
    background-color: #c0392b;
  }
  
  .status {
    font-weight: 600;
  }
  
  .status-present {
    color: #27ae60;
  }
  
  .status-exit {
    color: #e67e22;
  }
  
  .status-absent {
    color: #e74c3c;
  }
  
  .disabled {
    opacity: 0.6;
    pointer-events: none;
    cursor: not-allowed;
  }
  
  .timestamp {
    font-size: 0.85rem;
    color: #7f8c8d;
    margin-top: 4px;
  }
  
  .firebase-status {
    margin-top: 20px;
    padding: 12px;
    border-radius: 8px;
    background-color: #f8f9fa;
    display: inline-block;
  }
  
  .firebase-connected {
    color: #27ae60;
    font-weight: 600;
  }
  
  .firebase-disconnected {
    color: #e74c3c;
    font-weight: 600;
  }
</style>
</head>
<body>
  <div class="container">
    <h1>Attendance Terminal - ESP32</h1>
    <div class="current-time" id="currentTime">Loading time...</div>
    
    <table>
      <thead>
        <tr>
          <th>Person</th>
          <th>Entry</th>
          <th>Exit</th>
          <th>Absent</th>
          <th>Status</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>Person 1</td>
          <td><button id="entry1" class="btn btn-entry" onclick="pressEntry(1)">Entry</button></td>
          <td><button id="exit1" class="btn btn-exit" onclick="pressExit(1)" disabled>Exit</button></td>
          <td><button id="absent1" class="btn btn-absent" onclick="pressAbsent(1)">Absent</button></td>
          <td id="status1" class="status">-</td>
        </tr>
        <tr>
          <td>Person 2</td>
          <td><button id="entry2" class="btn btn-entry" onclick="pressEntry(2)">Entry</button></td>
          <td><button id="exit2" class="btn btn-exit" onclick="pressExit(2)" disabled>Exit</button></td>
          <td><button id="absent2" class="btn btn-absent" onclick="pressAbsent(2)">Absent</button></td>
          <td id="status2" class="status">-</td>
        </tr>
        <tr>
          <td>Person 3</td>
          <td><button id="entry3" class="btn btn-entry" onclick="pressEntry(3)">Entry</button></td>
          <td><button id="exit3" class="btn btn-exit" onclick="pressExit(3)" disabled>Exit</button></td>
          <td><button id="absent3" class="btn btn-absent" onclick="pressAbsent(3)">Absent</button></td>
          <td id="status3" class="status">-</td>
        </tr>
        <tr>
          <td>Person 4</td>
          <td><button id="entry4" class="btn btn-entry" onclick="pressEntry(4)">Entry</button></td>
          <td><button id="exit4" class="btn btn-exit" onclick="pressExit(4)" disabled>Exit</button></td>
          <td><button id="absent4" class="btn btn-absent" onclick="pressAbsent(4)">Absent</button></td>
          <td id="status4" class="status">-</td>
        </tr>
        <tr>
          <td>Person 5</td>
          <td><button id="entry5" class="btn btn-entry" onclick="pressEntry(5)">Entry</button></td>
          <td><button id="exit5" class="btn btn-exit" onclick="pressExit(5)" disabled>Exit</button></td>
          <td><button id="absent5" class="btn btn-absent" onclick="pressAbsent(5)">Absent</button></td>
          <td id="status5" class="status">-</td>
        </tr>
      </tbody>
    </table>
    
    <div class="firebase-status">
      Firestore Status: <span id="firebaseStatus" class="firebase-connected">Connected</span>
    </div>
  </div>

  <script>
    // Update current time
    function updateCurrentTime() {
      const now = new Date();
      const timeString = now.toLocaleTimeString([], {hour: '2-digit', minute:'2-digit', second:'2-digit'});
      const dateString = now.toLocaleDateString([], {weekday: 'long', year: 'numeric', month: 'long', day: 'numeric'});
      document.getElementById('currentTime').textContent = `${dateString} | ${timeString}`;
    }
    
    // Initialize and update time every second
    updateCurrentTime();
    setInterval(updateCurrentTime, 1000);
    
    // Disable all buttons for a person
    function disableAll(id) {
      document.getElementById(`entry${id}`).disabled = true;
      document.getElementById(`entry${id}`).classList.add("disabled");
      document.getElementById(`exit${id}`).disabled = true;
      document.getElementById(`exit${id}`).classList.add("disabled");
      document.getElementById(`absent${id}`).disabled = true;
      document.getElementById(`absent${id}`).classList.add("disabled");
    }
    
    // Send data to ESP32 endpoint
    function sendData(endpoint, id) {
      const xhr = new XMLHttpRequest();
      xhr.open("POST", endpoint, true);
      xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      xhr.send("id=" + id);
    }
    
    // Entry button handler
    function pressEntry(id) {
      document.getElementById(`status${id}`).innerHTML = 'Entry Marked<span class="timestamp">Processing...</span>';
      document.getElementById(`status${id}`).className = "status status-present";
      
      document.getElementById(`entry${id}`).disabled = true;
      document.getElementById(`entry${id}`).classList.add("disabled");
      document.getElementById(`absent${id}`).disabled = true;
      document.getElementById(`absent${id}`).classList.add("disabled");
      document.getElementById(`exit${id}`).disabled = false;
      document.getElementById(`exit${id}`).classList.remove("disabled");
      
      sendData("/entry", id);
    }
    
    // Exit button handler
    function pressExit(id) {
      document.getElementById(`status${id}`).innerHTML = 'Exit Marked<span class="timestamp">Processing...</span>';
      document.getElementById(`status${id}`).className = "status status-exit";
      
      disableAll(id);
      sendData("/exit", id);
    }
    
    // Absent button handler
    function pressAbsent(id) {
      document.getElementById(`status${id}`).innerHTML = 'Marked Absent<span class="timestamp">Processing...</span>';
      document.getElementById(`status${id}`).className = "status status-absent";
      
      disableAll(id);
      sendData("/absent", id);
    }
  </script>
</body>
</html>
)=====";

  server.send(200, "text/html", html);
}

void handleEntry() {
  int personId = server.arg("id").toInt();
  Serial.printf("Entry pressed for Person %d\n", personId);
  
  // Store entry time
  entryTimes[personId-1] = millis();
  
  // Get current ISO time
  String entryTime = getISOTime();
  
  // Update Firestore
  String documentPath = "Attendance/Person_" + String(personId);
  
  FirebaseJson content;
  content.set("fields/status/stringValue", "Present");
  content.set("fields/entryTime/stringValue", entryTime.c_str());
  
  String mask = "status,entryTime";
  
  if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", 
                                      documentPath.c_str(), content.raw(), mask)) {
    Serial.println("Entry recorded in Firestore");
    server.send(200, "text/plain", "Entry recorded");
  } else {
    Serial.println("Firestore error: " + fbdo.errorReason());
    server.send(500, "text/plain", "Error recording entry");
  }
}

void handleExit() {
  int personId = server.arg("id").toInt();
  Serial.printf("Exit pressed for Person %d\n", personId);
  
  // Calculate duration
  unsigned long duration = millis() - entryTimes[personId-1];
  String durationStr = formatDuration(duration);
  
  // Get current ISO time
  String exitTime = getISOTime();
  
  // Update Firestore
  String documentPath = "Attendance/Person_" + String(personId);
  
  FirebaseJson content;
  content.set("fields/exitTime/stringValue", exitTime.c_str());
  content.set("fields/duration/stringValue", durationStr.c_str());
  
  String mask = "exitTime,duration";
  
  if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", 
                                      documentPath.c_str(), content.raw(), mask)) {
    Serial.println("Exit recorded in Firestore");
    server.send(200, "text/plain", "Exit recorded");
  } else {
    Serial.println("Firestore error: " + fbdo.errorReason());
    server.send(500, "text/plain", "Error recording exit");
  }
}

void handleAbsent() {
  int personId = server.arg("id").toInt();
  Serial.printf("Absent pressed for Person %d\n", personId);
  
  // Update Firestore
  String documentPath = "Attendance/Person_" + String(personId);
  
  FirebaseJson content;
  content.set("fields/status/stringValue", "Absent");
  
  String mask = "status";
  
  if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", 
                                      documentPath.c_str(), content.raw(), mask)) {
    Serial.println("Absent recorded in Firestore");
    server.send(200, "text/plain", "Absent recorded");
  } else {
    Serial.println("Firestore error: " + fbdo.errorReason());
    server.send(500, "text/plain", "Error recording absent");
  }
}

// Helper function to get current time in ISO format
String getISOTime() {
  timeClient.update();
  time_t now = timeClient.getEpochTime();
  struct tm *timeinfo = gmtime(&now);
  
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
  return String(buffer);
}

// Helper function to get current date
String getCurrentDate() {
  timeClient.update();
  time_t now = timeClient.getEpochTime();
  struct tm *timeinfo = gmtime(&now);
  
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
  return String(buffer);
}

// Helper function to format duration as HH:MM:SS
String formatDuration(unsigned long ms) {
  unsigned long seconds = ms / 1000;
  unsigned long hours = seconds / 3600;
  seconds = seconds % 3600;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  
  char buffer[20];
  sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
  return String(buffer);
}

void tokenStatusCallback(TokenInfo info) {
  Serial.printf("Token Info: type = %d, status = %d\n", info.type, info.status);
}
