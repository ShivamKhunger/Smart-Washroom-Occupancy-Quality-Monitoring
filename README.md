# 🚽 Smart Washroom Occupany & Cleaniness Monitoring System (IoT-Based) 📡

An IoT project to monitor washroom **occupancy** and **cleanliness** in **public** or **shared washrooms** using low-cost embedded sensors and real-time cloud updates.

---

## 🧠 Use Case

This system is ideal for:

- 🏢 **Hostel/Common Area Washrooms**
- 🏬 **Public Malls**
- 🚌 **Transport Hubs (Airports, Stations)**
- 🏫 **School/College Campuses**

It helps users and maintenance staff by:

- ✅ Knowing in advance if a stall is **occupied or free**
- 🚨 Alerting cleaning staff when a washroom **needs cleaning** based on **air quality** or **high usage count**
- 📱 Viewing real-time status via **ThingSpeak** cloud dashboards or mobile app integrations

---

## 🔧 Hardware Used

- **ESP32-C6-WROOM-1** – microcontroller
- **IR Sensor** – to detect person entry/exit
- **MQ-135** – gas sensor for air quality
- **DHT22** – humidity and temperature sensor
- **16x2 LCD** – to display status (Occupied, Clean, Needs Cleaning)
- **Push Button** – to manually reset cleanliness alert

---

## ☁️ Features

- 👤 **Occupancy Detection:** Detects number of users (e.g., alerts after 7 people).
- 🧼 **Cleanliness Monitoring:** Based on gas/humidity levels (bad smell, high humidity).
- 📊 **Cloud Integration with ThingSpeak:** Sends real-time data for remote monitoring.
- 🔁 **Manual Reset:** Cleaning staff can reset alerts via physical button.
- 📟 **LCD Display:** Shows current status (Occupied/Available + Clean/Needs Cleaning).

---


