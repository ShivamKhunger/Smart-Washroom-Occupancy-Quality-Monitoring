#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "ThingSpeak.h"

// Pin Definitions
#define IR_SENSOR_PIN 13
#define MQ135_PIN 6
#define DHT_PIN 7
#define RESET_BUTTON_PIN 9

// Wi-Fi Credentials
const char* ssid = ""; //Wifi Name
const char* password = ""; //Wifi Pass

// ThingSpeak
WiFiClient client;
unsigned long channelID = ;     //thingspeak channel id
const char* writeAPIKey = "";   //thingspeak api key(write)
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// States
bool isOccupied = false;
bool prevIRState = false;
bool needsCleaning = false;
bool justCleaned = false;
int personCount = 0;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(4, 5);
  lcd.init(); lcd.backlight();
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  ThingSpeak.begin(client);

  lcd.setCursor(0, 0); lcd.print("Washroom Monitoring System");
  delay(2000);
  lcd.clear();
}

void loop() {
  // IR sensor: LOW = occupied
  bool irState = digitalRead(IR_SENSOR_PIN) == LOW;
  isOccupied = irState;

  if (!prevIRState && irState) {
    personCount++;
    Serial.println("Person entered. Count: " + String(personCount));
  }
  prevIRState = irState;

  int gasLevel = analogRead(MQ135_PIN);
  float humidity = dht.readHumidity();

  // Push Button Reset
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    if (millis() - lastDebounceTime > 500) {
      needsCleaning = false;
      justCleaned = true;
      personCount = 0;
      Serial.println("Reset done. Status is Clean.");
      lastDebounceTime = millis();
    }
  }

  // Cleaning logic only if not just cleaned
  if (!needsCleaning && !justCleaned &&
      (gasLevel > 2000 || humidity > 85 || personCount >= 4)) {
    needsCleaning = true;
    Serial.println("Cleaning Required!");
  }

  // After some time, allow rechecking cleaning again
  if (justCleaned && millis() - lastDebounceTime > 10000) {
    justCleaned = false; // allow new triggers after 10 seconds
  }

  // LCD Output
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(isOccupied ? "Status: Occupied" : "Status: Free");
  lcd.setCursor(0, 1);
  lcd.print(needsCleaning ? "Needs Cleaning!" : "Clean");

  // Send to ThingSpeak
  ThingSpeak.setField(1, isOccupied ? 1 : 0);
  ThingSpeak.setField(2, needsCleaning ? 1 : 0);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, gasLevel);
  ThingSpeak.setField(5, personCount);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);
  Serial.println(status == 200 ? "✓ Data sent to cloud" : "✗ Failed to send data");

  delay(2000);
}
