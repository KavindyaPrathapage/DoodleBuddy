# 🎨 DoodleBuddy – Voice-Controlled Drawing Table

**DoodleBuddy** is a Human-Computer Interaction (HCI) project designed to assist users in adjusting a drawing table using **voice commands**. It features dual gear motors to control the table's angle and a rolling cover that moves according to object detection, making it ideal for hands-free, accessible drawing or writing sessions.

---

## 📽️ Project Demo

👉 Watch the project video [https://youtube.com/shorts/sW54Ou9ikaM?feature=share] 


---

## 🚀 Features

- 🎙️ **Voice Command Support** via Bluetooth (e.g., "upward", "downward", "stop")
- ⚙️ Dual **gear motor** control for angle adjustment and rolling mechanism
- 🧠 Object proximity detection with **ultrasonic sensors**
- 🔁 Automatic response to hand movement above the table
- 💻 Developed using **ESP32**, **Arduino**, and **BluetoothSerial**

---

## 🔧 Tech Stack

- **Microcontroller**: ESP32
- **Programming Language**: C++ (Arduino)
- **Motors**: Gear motors (L298 motor driver)
- **Sensors**: 3x HC-SR04 Ultrasonic Distance Sensors
- **Bluetooth**: BluetoothSerial library on ESP32
- **Development Tools**: VS Code + Arduino Extension

---

## 🧠 How It Works

1. **Motor 1** controls table angle via voice commands.
2. **Motor 2** rolls a cover in response to hand movement (up/down).
3. 3 Ultrasonic sensors detect hand position and send feedback to the ESP32.
4. Bluetooth module receives commands like `"upward"`, `"downward"`, or `"stop"`.

---



