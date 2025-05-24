# 🌱 Seed Germination Monitoring System

An Arduino Uno-based system to monitor key environmental parameters (temperature, humidity, soil moisture, and light intensity) and predict seed germination probability in real-time.

---

## 📦 Features
- Displays readings on a 16x2 LCD screen.
- Calculates germination probability using weighted sensor data.
- Serial output with alerts and probability breakdown.

---

## 🔧 Hardware Components
- Arduino Uno  
- DHT11 (Temperature + Humidity)  
- Capacitive Soil Moisture Sensor  
- LDR (Light Sensor)  
- I2C 16x2 LCD Display  
- Jumper Wires, Breadboard  

---

## 📁 Project Structure

| Folder          | Description                |
|-----------------|----------------------------|
| `code/`         | Arduino `.ino` sketch      |
| `report/`       | Detailed project report    |
| `presentation/` | Final PPT (as PDF)         |
| `circuit/`      | Circuit diagram            |
| `datasheets/`   | Sensor datasheets          |
| `video_demo/`   | Link to working demo video |

---

## 📺 Demo
**[Watch Video Demonstration](https://drive.google.com/drive/folders/1SHvoGaJ7IqajsUZvu3OeW52s2uBUqYsO?usp=sharing)**

---

## 📈 Germination Probability Formula

```math
P = 0.4 * T_factor + 0.3 * M_factor + 0.3 * L_factor
