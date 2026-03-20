#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Pin Definitions
#define mq135 A0
#define buzzer D1
#define redled D2
#define greenled D3
// Access Point credentials
const char* ssid = "AirMonitor_AP";
const char* password = "12345678";

// Threshold values
int warningThreshold = 350;  // Yellow zone
int dangerThreshold = 500;   // Red zone

// Create web server on port 80
ESP8266WebServer server(80);

// Variables for average calculation
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  Serial.begin(115200);

  // Initialize pins
  pinMode(buzzer, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(redled, LOW);
  digitalWrite(greenled, LOW);

  // Initialize readings array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Setup WiFi in AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Connect to WiFi: " + String(ssid));
  Serial.println("Password: " + String(password));

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/threshold", handleThreshold);
  server.on("/buzzer", handleBuzzer);
  server.on("/calibrate", handleCalibrate);
  server.on("/style.css", handleCSS);

  server.begin();
  Serial.println("HTTP server started");

  // Initial calibration (warm-up)
  Serial.println("Warming up MQ135 sensor...");
  delay(1000);
}

void loop() {
  server.handleClient();
  readSensor();
  checkAirQuality();
  delay(100);
}

void readSensor() {
  // Subtract the last reading
  total = total - readings[readIndex];

  // Read new sensor value
  readings[readIndex] = analogRead(mq135);

  // Add the reading to the total
  total = total + readings[readIndex];

  // Advance to the next position in the array
  readIndex = readIndex + 1;

  // If we're at the end of the array, wrap around
  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  // Calculate the average
  average = total / numReadings;
}

void checkAirQuality() {
  if (average > dangerThreshold) {
    // Danger zone - fast beeping
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);

    delay(50);
  } else if (average > warningThreshold) {
    // Warning zone - slow beeping
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    digitalWrite(redled, HIGH);
    digitalWrite(greenled, LOW);
    delay(200);
  } else {
    // Good air quality - buzzer off
    digitalWrite(buzzer, LOW);
    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);
  }
}

void handleCSS() {
  String css = "/* Modern Air Quality Monitor Styles */\n";
  css += ":root {\n";
  css += "  --primary-color: #4361ee;\n";
  css += "  --secondary-color: #3f37c9;\n";
  css += "  --success-color: #4cc9f0;\n";
  css += "  --warning-color: #f72585;\n";
  css += "  --danger-color: #b5179e;\n";
  css += "  --light-bg: #f8f9fa;\n";
  css += "  --dark-text: #212529;\n";
  css += "  --card-shadow: 0 10px 30px rgba(0,0,0,0.1);\n";
  css += "  --transition: all 0.3s ease;\n";
  css += "}\n\n";

  css += "* {\n";
  css += "  margin: 0;\n";
  css += "  padding: 0;\n";
  css += "  box-sizing: border-box;\n";
  css += "}\n\n";

  css += "body {\n";
  css += "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n";
  css += "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);\n";
  css += "  min-height: 100vh;\n";
  css += "  display: flex;\n";
  css += "  justify-content: center;\n";
  css += "  align-items: center;\n";
  css += "  padding: 20px;\n";
  css += "}\n\n";

  css += ".container {\n";
  css += "  max-width: 900px;\n";
  css += "  width: 100%;\n";
  css += "  margin: 0 auto;\n";
  css += "}\n\n";

  css += ".glass-card {\n";
  css += "  background: rgba(255, 255, 255, 0.95);\n";
  css += "  backdrop-filter: blur(10px);\n";
  css += "  border-radius: 30px;\n";
  css += "  padding: 40px;\n";
  css += "  box-shadow: var(--card-shadow);\n";
  css += "  border: 1px solid rgba(255, 255, 255, 0.2);\n";
  css += "}\n\n";

  css += ".header {\n";
  css += "  text-align: center;\n";
  css += "  margin-bottom: 40px;\n";
  css += "}\n\n";

  css += ".header h1 {\n";
  css += "  font-size: 2.5em;\n";
  css += "  color: var(--dark-text);\n";
  css += "  margin-bottom: 10px;\n";
  css += "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);\n";
  css += "  -webkit-background-clip: text;\n";
  css += "  -webkit-text-fill-color: transparent;\n";
  css += "}\n\n";

  css += ".header p {\n";
  css += "  color: #666;\n";
  css += "  font-size: 1.1em;\n";
  css += "}\n\n";

  css += ".network-badge {\n";
  css += "  display: inline-block;\n";
  css += "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);\n";
  css += "  color: white;\n";
  css += "  padding: 8px 20px;\n";
  css += "  border-radius: 50px;\n";
  css += "  font-size: 0.9em;\n";
  css += "  margin-top: 10px;\n";
  css += "  box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4);\n";
  css += "}\n\n";

  css += ".main-value {\n";
  css += "  text-align: center;\n";
  css += "  margin-bottom: 30px;\n";
  css += "}\n\n";

  css += ".value-display {\n";
  css += "  font-size: 5em;\n";
  css += "  font-weight: bold;\n";
  css += "  padding: 20px;\n";
  css += "  border-radius: 30px;\n";
  css += "  transition: var(--transition);\n";
  css += "  background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);\n";
  css += "  color: white;\n";
  css += "  box-shadow: 0 20px 40px rgba(0,0,0,0.2);\n";
  css += "  margin: 20px 0;\n";
  css += "}\n\n";

  css += ".value-label {\n";
  css += "  font-size: 0.3em;\n";
  css += "  display: block;\n";
  css += "  margin-top: 10px;\n";
  css += "  opacity: 0.9;\n";
  css += "}\n\n";

  css += ".stats-grid {\n";
  css += "  display: grid;\n";
  css += "  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));\n";
  css += "  gap: 20px;\n";
  css += "  margin: 30px 0;\n";
  css += "}\n\n";

  css += ".stat-card {\n";
  css += "  background: var(--light-bg);\n";
  css += "  padding: 25px;\n";
  css += "  border-radius: 20px;\n";
  css += "  text-align: center;\n";
  css += "  transition: var(--transition);\n";
  css += "  box-shadow: 0 5px 15px rgba(0,0,0,0.05);\n";
  css += "}\n\n";

  css += ".stat-card:hover {\n";
  css += "  transform: translateY(-5px);\n";
  css += "  box-shadow: 0 10px 25px rgba(0,0,0,0.1);\n";
  css += "}\n\n";

  css += ".stat-icon {\n";
  css += "  font-size: 2.5em;\n";
  css += "  margin-bottom: 15px;\n";
  css += "}\n\n";

  css += ".stat-value {\n";
  css += "  font-size: 2em;\n";
  css += "  font-weight: bold;\n";
  css += "  color: var(--primary-color);\n";
  css += "  margin: 10px 0;\n";
  css += "}\n\n";

  css += ".stat-label {\n";
  css += "  color: #666;\n";
  css += "  font-size: 0.9em;\n";
  css += "  text-transform: uppercase;\n";
  css += "  letter-spacing: 1px;\n";
  css += "}\n\n";

  css += ".quality-badge {\n";
  css += "  display: inline-block;\n";
  css += "  padding: 10px 30px;\n";
  css += "  border-radius: 50px;\n";
  css += "  font-weight: bold;\n";
  css += "  font-size: 1.2em;\n";
  css += "  text-transform: uppercase;\n";
  css += "  letter-spacing: 2px;\n";
  css += "  margin: 20px 0;\n";
  css += "}\n\n";

  css += ".quality-good { background: #4cc9f0; color: white; }\n";
  css += ".quality-warning { background: #f72585; color: white; }\n";
  css += ".quality-danger { background: #b5179e; color: white; }\n\n";

  css += ".threshold-section {\n";
  css += "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);\n";
  css += "  border-radius: 20px;\n";
  css += "  padding: 30px;\n";
  css += "  margin: 30px 0;\n";
  css += "  color: white;\n";
  css += "}\n\n";

  css += ".threshold-title {\n";
  css += "  font-size: 1.5em;\n";
  css += "  margin-bottom: 20px;\n";
  css += "  text-align: center;\n";
  css += "}\n\n";

  css += ".threshold-inputs {\n";
  css += "  display: flex;\n";
  css += "  gap: 20px;\n";
  css += "  flex-wrap: wrap;\n";
  css += "  justify-content: center;\n";
  css += "}\n\n";

  css += ".input-group {\n";
  css += "  flex: 1;\n";
  css += "  min-width: 200px;\n";
  css += "}\n\n";

  css += ".input-group label {\n";
  css += "  display: block;\n";
  css += "  margin-bottom: 8px;\n";
  css += "  font-size: 0.9em;\n";
  css += "  opacity: 0.9;\n";
  css += "}\n\n";

  css += ".threshold-input {\n";
  css += "  width: 100%;\n";
  css += "  padding: 12px;\n";
  css += "  border: none;\n";
  css += "  border-radius: 10px;\n";
  css += "  font-size: 1em;\n";
  css += "  background: rgba(255,255,255,0.2);\n";
  css += "  color: white;\n";
  css += "  border: 2px solid rgba(255,255,255,0.1);\n";
  css += "}\n\n";

  css += ".threshold-input:focus {\n";
  css += "  outline: none;\n";
  css += "  border-color: white;\n";
  css += "}\n\n";

  css += ".btn {\n";
  css += "  padding: 15px 30px;\n";
  css += "  border: none;\n";
  css += "  border-radius: 50px;\n";
  css += "  font-size: 1em;\n";
  css += "  font-weight: bold;\n";
  css += "  cursor: pointer;\n";
  css += "  transition: var(--transition);\n";
  css += "  text-transform: uppercase;\n";
  css += "  letter-spacing: 1px;\n";
  css += "  box-shadow: 0 5px 15px rgba(0,0,0,0.2);\n";
  css += "}\n\n";

  css += ".btn-primary {\n";
  css += "  background: white;\n";
  css += "  color: var(--primary-color);\n";
  css += "}\n\n";

  css += ".btn-primary:hover {\n";
  css += "  transform: translateY(-2px);\n";
  css += "  box-shadow: 0 8px 25px rgba(255,255,255,0.3);\n";
  css += "}\n\n";

  css += ".btn-success {\n";
  css += "  background: #4cc9f0;\n";
  css += "  color: white;\n";
  css += "}\n\n";

  css += ".btn-danger {\n";
  css += "  background: #f72585;\n";
  css += "  color: white;\n";
  css += "}\n\n";

  css += ".button-group {\n";
  css += "  display: flex;\n";
  css += "  gap: 15px;\n";
  css += "  justify-content: center;\n";
  css += "  flex-wrap: wrap;\n";
  css += "  margin: 30px 0;\n";
  css += "}\n\n";

  css += ".footer {\n";
  css += "  text-align: center;\n";
  css += "  margin-top: 30px;\n";
  css += "  color: #666;\n";
  css += "  font-size: 0.9em;\n";
  css += "}\n\n";

  css += "@media (max-width: 768px) {\n";
  css += "  .glass-card { padding: 20px; }\n";
  css += "  .value-display { font-size: 3em; }\n";
  css += "  .stats-grid { grid-template-columns: 1fr; }\n";
  css += "}\n\n";

  css += "@keyframes pulse {\n";
  css += "  0% { transform: scale(1); }\n";
  css += "  50% { transform: scale(1.05); }\n";
  css += "  100% { transform: scale(1); }\n";
  css += "}\n\n";

  css += ".pulse {\n";
  css += "  animation: pulse 2s infinite;\n";
  css += "}\n";

  server.send(200, "text/css", css);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>AirQuality Pro • Smart Monitor</title>";
  html += "<link rel='stylesheet' href='/style.css'>";
  html += "<script>";
  html += "function updateData() {";
  html += "  fetch('/data').then(response => response.json()).then(data => {";
  html += "    document.getElementById('ppm').innerHTML = data.ppm;";
  html += "    document.getElementById('avg').innerHTML = data.average;";
  html += "    document.getElementById('quality').innerHTML = data.quality;";
  html += "    document.getElementById('thresholds').innerHTML = ' ' + data.warning + ' | ' + data.danger;";
  html += "    var valueDisplay = document.getElementById('valueDisplay');";
  html += "    valueDisplay.style.background = data.class === 'good' ? 'linear-gradient(135deg, #4cc9f0 0%, #4361ee 100%)' : ";
  html += "                           data.class === 'warning' ? 'linear-gradient(135deg, #f72585 0%, #b5179e 100%)' : ";
  html += "                           'linear-gradient(135deg, #b5179e 0%, #7209b7 100%)';";
  html += "    document.getElementById('qualityBadge').className = 'quality-badge quality-' + data.class;";
  html += "  });";
  html += "}";
  html += "function updateThreshold() {";
  html += "  var warning = document.getElementById('warningThres').value;";
  html += "  var danger = document.getElementById('dangerThres').value;";
  html += "  fetch('/threshold?warning=' + warning + '&danger=' + danger).then(response => {";
  html += "    updateData();";
  html += "    showNotification('Thresholds updated successfully!', 'success');";
  html += "  });";
  html += "}";
  html += "function toggleBuzzer() {";
  html += "  fetch('/buzzer').then(response => {";
  html += "    showNotification('Buzzer test complete!', 'info');";
  html += "    updateData();";
  html += "  });";
  html += "}";
  html += "function calibrate() {";
  html += "  fetch('/calibrate').then(response => {";
  html += "    showNotification('Sensor calibrated successfully!', 'success');";
  html += "    updateData();";
  html += "  });";
  html += "}";
  html += "function showNotification(message, type) {";
  html += "  const notification = document.createElement('div');";
  html += "  notification.style.cssText = 'position:fixed;top:20px;right:20px;padding:15px 25px;background:' + ";
  html += "    (type === 'success' ? '#4cc9f0' : '#f72585') + ';color:white;border-radius:10px;box-shadow:0 5px 15px rgba(0,0,0,0.2);z-index:1000;animation:slideIn 0.3s ease;';";
  html += "  notification.innerHTML = message;";
  html += "  document.body.appendChild(notification);";
  html += "  setTimeout(() => { notification.remove(); }, 3000);";
  html += "}";
  html += "setInterval(updateData, 1000);";
  html += "window.onload = updateData;";
  html += "</script>";
  html += "<style>";
  html += "@keyframes slideIn { from { transform: translateX(100%); opacity: 0; } to { transform: translateX(0); opacity: 1; } }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<div class='glass-card'>";
  html += "<div class='header'>";
  html += "<h1> AirQuality Pro</h1>";
  html += "<p>Smart Air Pollution Monitoring System</p>";
  html += "<div class='network-badge'>";
  html += " " + String(ssid) + " | " + WiFi.softAPIP().toString() + "";
  html += "</div>";
  html += "</div>";

  html += "<div class='main-value'>";
  html += "<div id='valueDisplay' class='value-display'>";
  html += "<span id='ppm'>0</span>";
  html += "<span class='value-label'>PPM</span>";
  html += "</div>";
  html += "<div id='qualityBadge' class='quality-badge quality-good'>Good</div>";
  html += "</div>";

  html += "<div class='stats-grid'>";
  html += "<div class='stat-card'>";
  html += "<div class='stat-icon'></div>";
  html += "<div class='stat-label'>Average (10 readings)</div>";
  html += "<div class='stat-value'><span id='avg'>0</span> PPM</div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-icon'></div>";
  html += "<div class='stat-label'>Current Quality</div>";
  html += "<div class='stat-value'><span id='quality'>Good</span></div>";
  html += "</div>";

  html += "<div class='stat-card'>";
  html += "<div class='stat-icon'></div>";
  html += "<div class='stat-label'>Thresholds</div>";
  html += "<div class='stat-value'><span id='thresholds'></span></div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='threshold-section'>";
  html += "<div class='threshold-title'> Configure Thresholds</div>";
  html += "<div class='threshold-inputs'>";
  html += "<div class='input-group'>";
  html += "<label> Warning Level</label>";
  html += "<input type='number' class='threshold-input' id='warningThres' value='" + String(warningThreshold) + "'>";
  html += "</div>";
  html += "<div class='input-group'>";
  html += "<label> Danger Level</label>";
  html += "<input type='number' class='threshold-input' id='dangerThres' value='" + String(dangerThreshold) + "'>";
  html += "</div>";
  html += "<div class='input-group' style='display: flex; align-items: flex-end;'>";
  html += "<button class='btn btn-primary' onclick='updateThreshold()' style='width: 100%;'>Update</button>";
  html += "</div>";
  html += "</div>";
  html += "</div>";

  html += "<div class='button-group'>";
  html += "<button class='btn btn-success' onclick='calibrate()'> Calibrate Sensor</button>";
  html += "<button class='btn btn-danger' onclick='toggleBuzzer()'> Test Buzzer</button>";
  html += "</div>";

  html += "<div class='footer'>";
  html += "<p> Connected to: " + String(ssid) + " | Password: " + String(password) + "</p>";
  html += "<p> Real-time monitoring • Auto-updates every second</p>";
  html += "<p style='margin-top: 10px;'> Made with ESP8266 & MQ135</p>";
  html += "</div>";

  html += "</div>";
  html += "</div>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleData() {
  String quality;
  String className;

  if (average > dangerThreshold) {
    quality = "DANGER!";
    className = "danger";
  } else if (average > warningThreshold) {
    quality = "WARNING";
    className = "warning";
  } else {
    quality = "Good";
    className = "good";
  }

  String json = "{";
  json += "\"ppm\":" + String(analogRead(mq135)) + ",";
  json += "\"average\":" + String(average) + ",";
  json += "\"quality\":\"" + quality + "\",";
  json += "\"class\":\"" + className + "\",";
  json += "\"warning\":" + String(warningThreshold) + ",";
  json += "\"danger\":" + String(dangerThreshold);
  json += "}";

  server.send(200, "application/json", json);
}

void handleThreshold() {
  if (server.hasArg("warning")) {
    warningThreshold = server.arg("warning").toInt();
  }
  if (server.hasArg("danger")) {
    dangerThreshold = server.arg("danger").toInt();
  }
  server.send(200, "text/plain", "Thresholds updated");
}

void handleBuzzer() {
  // Test buzzer for 1 second
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  server.send(200, "text/plain", "Buzzer test complete");
}

void handleCalibrate() {
  // Simple calibration - takes 10 readings and sets baseline
  int calibrationSum = 0;
  for (int i = 0; i < 50; i++) {
    calibrationSum += analogRead(mq135);
    delay(10);
  }
  int calibrationValue = calibrationSum / 50;

  // Adjust thresholds based on calibration
  warningThreshold = calibrationValue + 50;
  dangerThreshold = calibrationValue + 150;

  server.send(200, "text/plain", "Calibration complete");
}