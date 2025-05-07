# Smart Water Feeder System

This project is an **IoT-based automated water feeder system** using the **ESP8266**, **Sinric Pro**, **servo motor**, **LCD display**, and a **water level sensor**. It allows remote control of a servo-based feeder and automates water pump control based on water level.

## Features

* 📶 Wi-Fi connectivity with ESP8266
* 📲 Smart control using **Sinric Pro**
* 💧 Automatic water pump control based on level thresholds
* 🔄 Servo motor to control feeder (e.g., open/close lid)
* 📟 LCD display showing real-time water level

## Components Used

* NodeMCU (ESP8266)
* Servo Motor (SG90 or similar)
* Water level sensor (analog)
* 16x2 I2C LCD display
* Relay Module
* External Pump (controlled via relay)

## Libraries Required

* `ESP8266WiFi.h`
* `SinricPro.h`
* `SinricProSwitch.h`
* `Wire.h`
* `LiquidCrystal_I2C.h`
* `Servo.h`

## Wiring Overview

| Component    | ESP8266 Pin |
| ------------ | ----------- |
| Servo Signal | D4          |
| Water Sensor | A0          |
| Relay IN     | D2 (GPIO2)  |
| LCD SDA      | D1          |
| LCD SCL      | D2          |

> ⚠️ Double check I2C address of LCD (usually 0x27 or 0x3F).

## Sinric Pro Setup

1. Create an account at [https://portal.sinric.pro](https://portal.sinric.pro)
2. Add a "Switch" device
3. Copy the **App Key**, **App Secret**, and **Device ID**
4. Replace them in the code

## How It Works

* The device connects to Wi-Fi and Sinric Pro
* It listens for power state changes (ON/OFF) to open/close the feeder
* It constantly monitors the water level via analog sensor:

  * Turns ON the pump if below `LOW_LEVEL`
  * Turns OFF the pump if above `HIGH_LEVEL`
* Displays water level on LCD

## Setup

1. Install the required libraries in Arduino IDE
2. Connect components as per wiring table
3. Upload the code from `smart_water_feeder_system.ino`
4. Monitor the Serial Monitor at 9600 baud for logs

## License

This project is open-source and available under the [MIT License](LICENSE).

---

Made with ❤️ using ESP8266 + Sinric Pro + Arduino

