#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Fingerprint.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#define ON_Board_LED 2 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Use hardware serial for ESP32
HardwareSerial mySerial(1);  // Use UART1 for communication with the fingerprint sensor (TX1, RX1)

// Define pin numbers for fingerprint sensor
#define RX_PIN 16 // RX pin for ESP32 (R307 TX)
#define TX_PIN 17 // TX pin for ESP32 (R307 RX)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String name;
String id;

const char* ssid = " ";   //Replace with your wifi ssid
const char* password = " ";   //Replace with your password

const char* host = "script.google.com"; 
const int httpsPort = 443;

WiFiClientSecure client;
String GAS_ID = " "; // Replace with your actual Google Apps Script ID
const int buzzer = 13;  // Use GPIO pin 13 for the buzzer

void setup() {
  Serial.begin(115200);
  
  pinMode(buzzer, OUTPUT);
  
  // Start Wi-Fi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.print("Successfully connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setInsecure();
  
  // Start communication with fingerprint sensor
  mySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN); 

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor detected!");
  } else {
    Serial.println("Fingerprint sensor not detected.");
    while (1) { delay(1); }
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Place your finger");
  display.display();
}

void loop() {
  getFingerprintID();
  delay(50);

  fingerprintID = getFingerprintIDez();
  
  if (fingerprintID == 1) {
    name = "Anirudh";
    id = "1";
    tone(buzzer, 2000); 
    delay(1000);
    noTone(buzzer);
    displayUserGreeting(name);
    sendToGoogleSheets();
  } else if (fingerprintID == 2) {
    name = "Anirudh_Prabhu";         //Use underscore inbetween name and do not use space 
    id = "2";
    tone(buzzer, 2000); 
    delay(1000);
    noTone(buzzer);
    displayUserGreeting(name);
    sendToGoogleSheets();
  } // Keep creating else if statements for each name 
   

}

void sendToGoogleSheets() {
  int maxRetries = 5;
  bool success = false;

  for (int attempt = 0; attempt < maxRetries && !success; attempt++) {
    if (!client.connect(host, httpsPort)) {
      Serial.println("Connection failed");
      displayConnectionStatus("Retry");
      delay(1000);
      continue;
    }

    String url = "/macros/s/" + GAS_ID + "/exec?value1=" + id + "&value2=" + name;
    Serial.print("Requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long start = millis();
    while (client.connected() && millis() - start < 5000) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Headers received");
        displayConnectionStatus("Data Sent");
        success = true;
        break;
      }
    }

    if (!success) {
      Serial.println("Failed to get response, retrying...");
      displayConnectionStatus("Retry");
      delay(1000);
    }
  }

  if (!success) {
    Serial.println("All attempts failed.");
    displayConnectionStatus("Failed");
  }
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return p;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return p;

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence "); Serial.println(finger.confidence);
  } else {
    Serial.println("Fingerprint not found");
  }
  return finger.fingerID;
}

int getFingerprintIDez() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;

  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence "); Serial.println(finger.confidence);
  return finger.fingerID;
}

void displayUserGreeting(String name) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(name);
  display.display();
  delay(5000);
  display.clearDisplay();
  display.display();
}

void displayConnectionStatus(String status) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(status);
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
}
