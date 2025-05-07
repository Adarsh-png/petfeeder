#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Wi-Fi Credentials
const char* WIFI_SSID = "realme";
const char* WIFI_PASS = "12345678";

// Sinric Pro Credentials
#define APP_KEY     "177ac0f0-5ff2-468e-9469-6c73dfd3961a"
#define APP_SECRET  "9f748b01-eb7a-45d5-bc89-145815b85fb2-00f9f61b-ef6f-428d-aeeb-1c43f3483f23"
#define DEVICE_ID   "67c5b514c8ff966556a1afeb"

// LCD Setup (I2C Address might be 0x3F or 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo Configuration
#define SERVO_PIN D4
Servo feederServo;
#define OPEN_ANGLE 180
#define CLOSE_ANGLE 0

// Water level monitoring
#define WATER_SENSOR A0
#define RELAY_PIN 2
const int LOW_LEVEL = 350;
const int HIGH_LEVEL = 450;

bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == DEVICE_ID) {
    Serial.printf("Device %s turned %s\r\n", deviceId.c_str(), state ? "on" : "off");
    feederServo.write(state ? OPEN_ANGLE : CLOSE_ANGLE); // Open or close feeder
  }
  return true;
}

void setupWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupSinricPro() {
  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.onConnected([]() { Serial.println("Connected to SinricPro"); });
  SinricPro.onDisconnected([]() { Serial.println("Disconnected from SinricPro"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

void setup() {
  Serial.begin(9600);

  // Pin setup
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start with pump OFF

  // LCD setup
  lcd.begin();
  lcd.backlight();
  lcd.print("Starting...");

  // Servo setup
  feederServo.attach(SERVO_PIN);
  feederServo.write(CLOSE_ANGLE);

  // Wi-Fi & Sinric setup
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();

  int waterLevel = analogRead(WATER_SENSOR);
  Serial.print("Water Level: ");
  Serial.println(waterLevel);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water Level:");
  lcd.setCursor(0, 1);
  lcd.print(waterLevel);

  if (waterLevel < LOW_LEVEL) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Pump ON");
  } else if (waterLevel > HIGH_LEVEL) {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pump OFF");
  }

  delay(1000);
}
