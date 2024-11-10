This project is a biometric attendance or authentication system using an ESP32, R307 fingerprint sensor, SSD1306 OLED display, and Google Sheets integration. It detects fingerprints and sends data (name and ID) to a Google Sheet via Wi-Fi, making it suitable for applications like access control or attendance tracking.

Key Features
  Fingerprint Detection: Uses an R307 fingerprint sensor for reliable identification.
  OLED Display Feedback: Provides real-time user feedback for fingerprint detection and data transmission status.
  Wi-Fi Connectivity: Connects to a specified Wi-Fi network to transmit data securely.
  Google Sheets Logging: Utilizes Google Apps Script for sending fingerprint data (name and ID) to a Google Sheet for easy tracking and storage.
  Buzzer Notification: Gives audible feedback using a buzzer upon successful fingerprint detection.
  Retry Mechanism: Automatically retries sending data in case of failed attempts.
Libraries Required
  Adafruit GFX Library
  Adafruit SSD1306 Library
  Adafruit Fingerprint Sensor Library
  WiFi and WiFiClientSecure libraries for ESP32.
Setup Instructions
  Fingerprint Sensor: Connect the RX and TX pins of the R307 sensor to the ESP32.
  OLED Display: Connect the OLED display to the ESP32.
  Wi-Fi: Enter your Wi-Fi credentials in the ssid and password variables.
  Google Apps Script: Set up a Google Apps Script and enter the script ID in the GAS_ID variable.
  Buzzer: Connect a buzzer to GPIO pin 13 of the ESP32.
This setup provides a user-friendly, Wi-Fi-enabled biometric system with on-screen and audible feedback.
