# 500W-IOT-BASED-SOLAR-GENERATING-STATION
500W IOT BASED SOLAR GENERATING STATION
This project involves the development of an IoT-based solar inverter monitoring system using an ESP32/ESP8266 microcontroller. The system monitors various parameters such as AC voltage, current, battery voltage, and power (wattage). These parameters are displayed on an LCD and are also sent to Firebase Realtime Database for remote monitoring. The system can also control a relay based on the state retrieved from Firebase.The code that was used was written as an ardunio sketch using c programming language.

# Components Used
ESP32/ESP8266
ACS712 Current Sensor
Voltage Sensors
Relay Module
LCD Display with I2C
Firebase Realtime Database
Wi-Fi Network

# Libraries Required
Wire.h
Arduino.h
LiquidCrystal_I2C.h
WiFi.h (for ESP32) or ESP8266WiFi.h (for ESP8266)
Firebase_ESP_Client.h
ACS712.h
# Connections
LCD Display: Connected via I2C to the ESP32/ESP8266.
ACS712 Current Sensor: Connected to analog pin GPIO34.
Voltage Sensors:
Battery voltage sensing connected to GPIO36.
AC voltage sensing connected to GPIO39.
Relay Module: Connected to digital pin GPIO25.
# Setup Instructions
Install Required Libraries: Install the necessary libraries via Arduino Library Manager.
Wi-Fi Configuration: Replace WIFI_SSID and WIFI_PASSWORD with your Wi-Fi credentials.
# Firebase Configuration:
Replace API_KEY with your Firebase project's API key.
Replace DATABASE_URL with your Firebase Realtime Database URL.
Upload the Code: Upload the provided code to your ESP32/ESP8266 board.
