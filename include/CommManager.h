#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Firebase_ESP_Client.h>
#include "config.h"

// Firebase helpers
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

class CommManager {
public:
    CommManager();
    void begin();
    void sendWhatsAppAlert(String message, bool &messageSentFlag);
    void uploadData(String poleId, bool isEarthOk, bool isLeakageOk);
    bool isConnected();

private:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig fb_config;
    bool signupOK = false;

    void setupWiFi();
    void setupFirebase();
};

#endif
