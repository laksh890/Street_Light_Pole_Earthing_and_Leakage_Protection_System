#include "CommManager.h"

CommManager::CommManager() {}

void CommManager::begin() {
    setupWiFi();
    setupFirebase();
}

void CommManager::setupWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
}

void CommManager::setupFirebase() {
    fb_config.api_key = FIREBASE_API_KEY;
    fb_config.database_url = FIREBASE_DATABASE_URL;

    if (Firebase.signUp(&fb_config, &auth, "", "")) {
        Serial.println("Firebase Sign Up OK");
        signupOK = true;
    } else {
        Serial.printf("Firebase Sign Up Failed: %s\n", fb_config.signer.signupError.message.c_str());
    }

    fb_config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&fb_config, &auth);
    Firebase.reconnectWiFi(true);
}

void CommManager::sendWhatsAppAlert(String message, bool &messageSentFlag) {
    if (WiFi.status() == WL_CONNECTED && !messageSentFlag) {
        HTTPClient http;

        String url = "https://api.ultramsg.com/" + String(ULTRAMSG_INSTANCE_ID) + "/messages/chat?token=" + String(ULTRAMSG_TOKEN);
        url += "&to=" + String(RECIPIENT_NUMBER);
        url += "&body=Critical%20Malfunction%3A%0ALocation%20-%20MIT%20ADT%20%2C%20Raj%20Lane%0APole%20Number%20-%20%23001%0AReason%20-%20" + message;
        url += "&priority=10";

        Serial.println("Sending WhatsApp alert...");
        http.begin(url);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            Serial.println("WhatsApp Alert Sent Successfully");
            messageSentFlag = true;
        } else {
            Serial.printf("WhatsApp Alert Failed: %d\n", httpResponseCode);
        }
        http.end();
    }
}

void CommManager::uploadData(String poleId, bool isEarthOk, bool isLeakageOk) {
    if (!signupOK) return;

    FirebaseJson json;
    json.add("poleid", poleId);
    json.add("isEarthOk", isEarthOk);
    json.add("isLeakageOk", isLeakageOk);

    String path = "/pole_data/pole" + poleId;
    if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) {
        Serial.printf("Data for pole %s uploaded to Firebase\n", poleId.c_str());
    } else {
        Serial.printf("Firebase Upload Failed: %s\n", fbdo.errorReason().c_str());
    }
}

bool CommManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
