#include "EmonLib.h" // Include Emon Library
#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Wi-Fi Credentials
const char* WIFI_SSID = "fedora";
const char* WIFI_PASSWORD = "leakbusters";

// UltraMsg API Credentials
const char* instance_id = "instance101625"; // Your instance ID
const char* token = "oba0b64kg7vo33jt";     // Your UltraMsg token
const char* to_number = "+917006456359";    // Recipient's WhatsApp number

// Firebase Credentials
const char* API_KEY = "AIzaSyDuUk48bude5QD4-WuYC_5IBOzflI1LF_k";
const char* DATABASE_URL = "https://leakage-fault-detection-default-rtdb.asia-southeast1.firebasedatabase.app/";

// Pin Definitions
#define V1 3.3
#define V2_PIN 33
#define ALARM_PIN 18
#define RELAY_PIN 27

int r1 = 2000;
bool critical = false; // Global variable to track critical state
bool sent = false;

EnergyMonitor emon1; // Create an instance
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);

  pinMode(V2_PIN, INPUT);
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);

  emon1.current(34, 0.0368); // Current: input pin, calibration.

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  // Firebase Setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase Sign Up OK");
    signupOK = true;
  } else {
    Serial.printf("Firebase Sign Up Failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void sendMessage(String message) {
  if (WiFi.status() == WL_CONNECTED && !sent) {
    HTTPClient http;

    // Construct the UltraMsg API URL
    String url = "https://api.ultramsg.com/";
    url += instance_id;
    url += "/messages/chat?token=";
    url += token;
    url += "&to=" + String(to_number);
    url += "&body=Critical%20Malfunction%3A%0ALocation%20-%20MIT%20ADT%20%2C%20Raj%20Lane%0APole%20Number%20-%20%23001%0AReason%20-%20" + message;
    url += "&priority=10"; // Optional priority parameter
    Serial.println("Request URL: " + url);

    // Initialize HTTP request
    http.begin(url);

    // Send GET request
    int httpResponseCode = http.GET();

    // Handle response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Message sent successfully!");
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    sent = true;
    http.end(); // Free resources
  } else {
    Serial.println("Wi-Fi Disconnected");
  }
}

void uploadToFirebase(String poleId, bool isEarthOk, bool isLeakageOk) {
  FirebaseJson json;
  json.add("poleid", poleId);
  json.add("isEarthOk", isEarthOk);
  json.add("isLeakageOk", isLeakageOk);

  String path = "pole_data/pole" + poleId;
  if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
    Serial.printf("Data for pole %s uploaded successfully\n", poleId.c_str());
  } else {
    Serial.printf("Failed to upload data for pole %s: %s\n", poleId.c_str(), fbdo.errorReason().c_str());
  }
}

void checkEarthingResistance() {
  int analog_value = analogRead(V2_PIN);
  float V2 = (analog_value / 4095.0) * 3.3;

  if (V1 == V2) {
    V2 -= 0.01; // Avoid division by zero
    Serial.println("Broken Circuit Detected");
    critical = true;
  }

  float ru = V2 / ((V1 - V2) / r1);
  Serial.print("Earthing Resistance: ");
  Serial.println(ru);

  if (ru > 3000) {
    critical = true;
    handleCriticalState();
    sendMessage("High Earthing Resistance Detected");
    uploadToFirebase("001", false, true);
    Serial.println("High earthing resistance detected!");
  }
}

void checkCurrentLeakage() {
  double Irms = emon1.calcIrms(1480);
  delay(500);
  Irms = 0;
  for (int i = 0; i < 4; i++) {
    Irms += emon1.calcIrms(1480) * 100; // Calculate Irms
    delay(10);
  }
  Irms = Irms / 4;
  Serial.print("Irms:");
  Serial.println(Irms);

  if (Irms > 3.03) {
    critical = true;
    handleCriticalState();
    sendMessage("High Leakage Current Detected");
    uploadToFirebase("001", true, false);
    Serial.println("High leakage current detected...");
  }
}

void handleCriticalState() {
  if (critical) {
    digitalWrite(ALARM_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Critical state! System halted.");
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'R') {
        critical = false;
        sent = false;
        Serial.println("System reset.");
      }
    }
  } else {
    digitalWrite(ALARM_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }
}

void loop() {
  if (!critical) {
    checkEarthingResistance();
    checkCurrentLeakage();
  }
  handleCriticalState();
  delay(1000);
}