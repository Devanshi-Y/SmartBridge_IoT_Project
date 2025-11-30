#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// ================= WiFi + ThingSpeak =================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";   
String apiKey = "Your_Thingspeak_Write_API_Key";  // <-- Your Write API Key
String server = "http://api.thingspeak.com/update";

// ================= Pin Definitions ====================
#define MOISTURE_PIN 34
#define SERVO1_PIN   4
#define SERVO2_PIN   5
#define BUZZER_PIN   27
#define TRIG_PIN     12
#define ECHO_PIN     13
#define IR_PIN       14

Servo s1, s2;

// ================= Flood Threshold ====================
// Soil sensor powered by 3.3V -> lower values
int moistureThreshold = 1500;  // Adjust using Serial Monitor

// ================= Helper Functions ====================
void bridgeUp() {
  s1.write(80);
  s2.write(80);
}

void bridgeDown() {
  s1.write(0);
  s2.write(0);
}

long readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.0343 / 2;  // Distance in cm
}

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  s1.attach(SERVO1_PIN);
  s2.attach(SERVO2_PIN);
  bridgeDown();

  // ================= Connect WiFi ======================
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  // ================= Read Sensors ======================
  int moisture = analogRead(MOISTURE_PIN);
  long distance = readUltrasonic();
  bool isFlood = (moisture < moistureThreshold);
  bool isShipUltrasonic = (distance < 20);
  bool isShipIR = (digitalRead(IR_PIN) == LOW);

  // ================= Decision Making ===================
  if (isFlood) {                      // Flood
    Serial.println("🚨 Flood Detected!");
    digitalWrite(BUZZER_PIN, HIGH);
    bridgeUp();
  }
  else if (isShipUltrasonic || isShipIR) {  // Ship Detected
    Serial.println("🚢 Ship Detected — Opening Bridge");
    digitalWrite(BUZZER_PIN, LOW);
    bridgeUp();
  }
  else {                              // Safe
    Serial.println("🌿 Safe — Bridge Down");
    digitalWrite(BUZZER_PIN, LOW);
    bridgeDown();
  }

  // ================= Status Values =====================
  int irValue = isShipIR ? 1 : 0;
  int bridgeState = (isFlood || isShipUltrasonic || isShipIR) ? 1 : 0;
  int buzzerState = digitalRead(BUZZER_PIN);
  int floodState = isFlood ? 1 : 0;

  // ================= Send to ThingSpeak =================
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server + "?api_key=" + apiKey +
                 "&field1=" + String(moisture) +
                 "&field2=" + String(distance) +
                 "&field3=" + String(irValue) +
                 "&field4=" + String(bridgeState) +
                 "&field5=" + String(buzzerState) +
                 "&field6=" + String(floodState);

    http.begin(url);
    http.GET();
    http.end();
    Serial.println("📡 Data Sent to ThingSpeak");
  }

  delay(1500);
}
