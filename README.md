# 🌫️ Air Quality Monitoring Device

An IoT-based Air Quality Monitoring system using the **MQ135 gas sensor** and **ESP8266 Wi-Fi module** that detects harmful gases in the surrounding environment and provides real-time alerts via visual and audio indicators.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Circuit Diagram](#circuit-diagram)
- [Pin Configuration](#pin-configuration)
- [Software Requirements](#software-requirements)
- [Installation & Setup](#installation--setup)
- [How It Works](#how-it-works)
- [Threshold Configuration](#threshold-configuration)
- [Project Structure](#project-structure)
- [Output](#output)
- [Future Enhancements](#future-enhancements)
- [License](#license)

---

## 📖 Overview

This project monitors the air quality in real time using the **MQ135 sensor**, which detects gases like CO₂, ammonia, benzene, smoke, and other harmful pollutants. The system uses an **ESP8266 NodeMCU** as the main controller and displays the air quality status through:

- 🟢 **Green LED** — Air quality is **Good / Safe**
- 🔴 **Red LED** — Air quality is **Poor / Dangerous**
- 🔔 **Buzzer** — Audible alert when pollution exceeds threshold
- 📡 **Wi-Fi (ESP8266)** — Optional: Send data to cloud / serial monitor

---

## ✨ Features

- Real-time air quality monitoring
- Visual indication using Red and Green LEDs
- Audio alert using a buzzer when air quality is poor
- Serial monitor output for debugging and data logging
- Adjustable gas concentration threshold
- Low cost and easy to build
- Wi-Fi capable for IoT integration (via ESP8266)

---

## 🔧 Hardware Components

| Component            | Quantity | Description                                      |
|----------------------|----------|--------------------------------------------------|
| ESP8266 NodeMCU      | 1        | Main microcontroller with built-in Wi-Fi         |
| MQ135 Gas Sensor     | 1        | Detects CO₂, NH₃, benzene, smoke, etc.           |
| Red LED              | 1        | Indicates poor/dangerous air quality             |
| Green LED            | 1        | Indicates good/safe air quality                  |
| Buzzer               | 1        | Audio alert for high pollution levels            |
| 220Ω Resistor        | 2        | Current limiting resistors for LEDs             |
| Breadboard           | 1        | For prototyping connections                      |
| Jumper Wires         | Several  | Male-to-male and male-to-female                  |
| USB Cable (Micro-B)  | 1        | For powering/programming the ESP8266             |

---

## 🔌 Pin Configuration

| Component       | ESP8266 Pin | Notes                          |
|-----------------|-------------|--------------------------------|
| MQ135 (AOUT)    | A0          | Analog output of gas sensor    |
| MQ135 (DOUT)    | D5          | Digital output (optional)      |
| MQ135 (VCC)     | 3.3V / 5V   | Power supply                   |
| MQ135 (GND)     | GND         | Ground                         |
| Green LED (+)   | D1          | Through 220Ω resistor          |
| Red LED (+)     | D2          | Through 220Ω resistor          |
| Buzzer (+)      | D3          | Active buzzer                  |
| LED/Buzzer (-)  | GND         | Ground                         |

> ⚠️ **Note:** The ESP8266 operates at 3.3V logic. Avoid connecting 5V signals directly to GPIO pins.

---

## 🛠️ Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (v1.8.x or later)
- ESP8266 Board Package for Arduino IDE
- Libraries:
  - `ESP8266WiFi.h` (included with ESP8266 board package)
  - `MQ135` library (optional, for calibrated PPM readings)

### Installing ESP8266 Board in Arduino IDE

1. Open Arduino IDE → **File → Preferences**
2. Add this URL to "Additional Boards Manager URLs":
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Go to **Tools → Board → Boards Manager**
4. Search for `esp8266` and install **"ESP8266 by ESP8266 Community"**

---

## 🚀 Installation & Setup

1. **Clone this repository:**
   ```bash
   git clone https://github.com/Koppulapraneeth/AirMonitorDevice.git
   cd AirMonitorDevice
   ```

2. **Open the project in Arduino IDE:**
   - Open the `.ino` file from the cloned folder

3. **Select the correct board and port:**
   - **Tools → Board → NodeMCU 1.0 (ESP-12E Module)**
   - **Tools → Port → COM_X** (your device port)

4. **Upload the code:**
   - Click the **Upload** button (→)

5. **Open Serial Monitor** at `9600` baud to view sensor readings.

---

## ⚙️ How It Works

```
[MQ135 Sensor]
     |
     | Analog Signal (0–1023)
     ↓
[ESP8266 NodeMCU]
     |
     ├──── Value < Threshold ──→ Green LED ON | Buzzer OFF | Serial: "Air Quality: Good"
     |
     └──── Value ≥ Threshold ──→ Red LED ON  | Buzzer ON  | Serial: "Air Quality: Poor!"
```

1. The **MQ135 sensor** continuously reads gas concentration from the analog pin.
2. The **ESP8266** reads this analog value (0–1023).
3. If the value exceeds a defined **threshold**, it:
   - Turns **ON** the Red LED
   - Activates the **Buzzer**
   - Prints a warning to the Serial Monitor
4. If the value is below the threshold:
   - Turns **ON** the Green LED
   - Keeps Buzzer **OFF**
   - Prints a safe status to the Serial Monitor

---

## 🎚️ Threshold Configuration

You can adjust the air quality threshold in the code:

```cpp
// Adjust this value based on your environment and calibration
int threshold = 400;  // Raw analog value (0–1023)
```

> **Tip:** Run the device in your environment first, observe the baseline readings in the Serial Monitor, and set the threshold slightly above the normal idle value.

---

## 📁 Project Structure

```
AirMonitorDevice/
│
├── AirMonitorDevice.ino      # Main Arduino sketch
├── README.md                 # Project documentation
└── circuit/
    └── circuit_diagram.png   # Wiring/circuit diagram (if available)
```

---

## 📊 Output

**Serial Monitor Output (9600 baud):**
```
Air Quality Monitor Started...
Sensor Value: 245 → Air Quality: Good ✅
Sensor Value: 251 → Air Quality: Good ✅
Sensor Value: 512 → Air Quality: Poor! ⚠️  [BUZZER ON]
Sensor Value: 489 → Air Quality: Poor! ⚠️  [BUZZER ON]
```

---

## 🔮 Future Enhancements

- [ ] Display readings on an **OLED / LCD screen**
- [ ] Send air quality data to **ThingSpeak / Blynk / Firebase**
- [ ] Add **DHT11/DHT22** for temperature & humidity monitoring
- [ ] Implement **data logging** with timestamps
- [ ] Build a **mobile app** dashboard for remote monitoring
- [ ] Add **multiple gas sensors** (MQ2, MQ7) for comprehensive detection
- [ ] Use **deep sleep mode** for battery-powered deployment

---

## 👤 Author

**Koppula Praneeth**
- GitHub: [@Koppulapraneeth](https://github.com/Koppulapraneeth)

---

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).

---

> 💡 *If you found this project helpful, please consider giving it a ⭐ on GitHub!*
