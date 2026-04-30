# ⚡ Smart Pole Fault Detection System

An IoT-based embedded system designed to detect **electrical faults in street lighting infrastructure**, including leakage current and poor earthing conditions.

This system enables **real-time monitoring, automated safety response, and remote alerting**, making it ideal for smart city deployments.

---

## 🚀 Overview

Urban electrical infrastructure is prone to hidden faults that can cause:
- ⚠️ Electric shocks
- 🔥 Fire hazards
- ⚡ Energy loss

This project provides a **low-cost, scalable solution** using ESP32 and cloud connectivity to continuously monitor pole health and take immediate action.

---

## 🧠 Key Features

- ⚡ **Leakage Current Detection**
- 🌍 **Earthing Resistance Monitoring**
- 🔔 **Instant WhatsApp Alerts (UltraMsg API)**
- ☁️ **Cloud Logging via Firebase**
- 🔌 **Automatic Power Cut-off using Relay**
- 📡 **WiFi-enabled real-time monitoring**
- 🔁 **Retry & Fault Handling Logic**

---
## 🚨 Fault Conditions
Condition	Trigger
Earthing Fault	Resistance > Threshold
Broken Line	Open circuit detected
Leakage Current	Current > Safe limit

---

## 🏗️ System Architecture
[ Sensors ] → [ ESP32 ] → [ Firebase RTDB ]
↓
[ Alert System ]
↓
WhatsApp API


---

## 🔧 Hardware Components

- ESP32 Microcontroller  
- Current Sensor (via EmonLib)  
- Voltage Divider Circuit (for earthing measurement)  
- Relay Module  
- Buzzer / Alarm  
- Power Supply Unit  

---

## 💻 Software Stack

- Embedded C++ (Arduino Framework)
- Firebase Realtime Database
- UltraMsg WhatsApp API
- ESP32 WiFi Stack

---

## 📁 Project Structure
smart-pole-iot/
│
├── firmware/
│ ├── main.ino
│ ├── config.h # (NOT uploaded)
│ ├── secrets.example.h # Template file
│
├── docs/
│ ├── architecture.png
│ ├── wiring_diagram.png
│
├── README.md
├── LICENSE
├── .gitignore


## 📦 Dependencies
WiFi (ESP32 Core)
HTTPClient
Firebase ESP Client
EmonLib
ArduinoJson (optional)
Wire (if I2C used)
SPI (if SPI used)

---

## 🧪 Future Improvements
📡 LoRa-based communication
📊 Web dashboard visualization
🤖 ML-based fault prediction
🔄 OTA firmware updates
⚡ Power optimization

---

## 🎯 Use Cases
Smart Cities
Industrial Safety Systems
Electrical Infrastructure Monitoring
Government Urban Projects

---

## 👨‍💻 Author

Lakshay Gandotra
Civil Engineering, NIT Srinagar