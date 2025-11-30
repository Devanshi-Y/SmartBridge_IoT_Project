# Smart Flood-Resistant Bridge System (ESP32 + IoT + Sensors)

A fully automated **Smart Bridge System** designed to detect **floods** and **approaching ships**, automatically lift the bridge using dual servos, trigger alerts, and send live sensor data to **ThingSpeak**, with a **custom dashboard** for visualization.

This project integrates **IoT, ESP32, sensors, real-time data monitoring, and frontend visualization**.

---

## Features

### Flood Detection
- Soil Moisture Sensor checks water level
- Automatically lifts the bridge during flood
- Buzzer alerts the surroundings
- Data uploaded to ThingSpeak

### Ship Detection
- Ultrasonic sensor measures ship distance  
- IR sensor detects ship presence  
- Bridge automatically lifts when a ship arrives  

### Automated Bridge Movement
- Two synchronized servo motors  
- Smooth lift and drop mechanism  

### ThingSpeak IoT Integration
Live data uploaded every few seconds:
- **Field 1 → Moisture / Water level**
- **Field 2 → Ship distance**
- **Field 3 → Alerts (optional)**

### Web-Based Dashboard
A frontend (HTML + CSS + JS) that:
- Shows real-time water level graph  
- Shows ship detection status  
- Highlights alerts (safe, warning, danger)  
- Displays animated ship indicator  
- Allows manual bridge controls (UI events only)

---

## Hardware Components

| Component | Purpose |
|----------|---------|
| **ESP32** | Main controller |
| **Soil Moisture Sensor** | Flood detection |
| **Ultrasonic Sensor (HC-SR04)** | Ship distance measurement |
| **IR Sensor** | Ship presence detection |
| **Servos x2** | Lift and lower the bridge |
| **Buzzer** | Alerts |
| **External 5V Supply (optional)** | Power for servos |
| **Jumper Wires** | Connections |

---

## Wiring / Connections

### ESP32 Pin Mapping

| Component | ESP32 Pin |
|----------|-----------|
| Servo 1 | GPIO 4 |
| Servo 2 | GPIO 5 |
| Buzzer | GPIO 27 |
| Soil Moisture OUT | GPIO 34 |
| Ultrasonic TRIG | GPIO 12 |
| Ultrasonic ECHO | GPIO 13 |
| IR Sensor OUT | GPIO 14 |

### Important Notes
- Servos must use **external 5V power**  
- All **GNDs must be connected together**
- Ultrasonic **ECHO pin (5V)** must use a **voltage divider** before entering ESP32 GPIO 13

---

## Project Structure

```
Smart-Bridge-Project/
│
├── Arduino/
│   └── smart_bridge.ino              # ESP32 code
│
├── Dashboard/
│   ├── index.html                    # UI layout
│   ├── style.css                     # Styling
│   └── script.js                     # Logic & ThingSpeak fetch
│
├── Media/
│   ├── thingspeak_demo.mp4           # Optional video demo
│   ├── wiring_diagram.png            # Circuit Diagram
│   └── working_pictures/             # Prototype Images
│
└── README.md                         # This file
```


---

## How the System Works

### 1️. Flood Detection  
- Soil moisture sensor reads water level  
- If moisture < threshold → Flood  
- Buzzer turns ON  
- Bridge lifts automatically  

### 2️. Ship Detection  
- Ultrasonic checks distance  
- IR sensor confirms ship presence  
- If distance < 20 cm OR IR detects → Ship approaching  
- Bridge lifts  

### 3️. Safe Mode  
- No ship  
- No flood  
- Bridge stays down  
- Buzzer OFF  

### 4️. IoT Upload  
ESP32 sends:
- Moisture value  
- Ultrasonic distance  
to ThingSpeak every cycle.

Dashboard fetches this data every 5 seconds.

---

## ThingSpeak Setup

Create channel with fields:

| Field | Data |
|-------|------|
| Field 1 | Moisture level |
| Field 2 | Ship distance |
| Field 3 | Alerts (optional) |

Update your `script.js`:

```js
const channelID = YOUR_CHANNEL_ID;
const readAPIKey = "YOUR_READ_API_KEY";
