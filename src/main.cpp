#include <Arduino.h>
#include "config.h"
#include "SensorManager.h"
#include "CommManager.h"
#include "SafetySystem.h"

// Global Modules
SensorManager sensors;
CommManager comms;
SafetySystem safety;

// Global State
bool messageSent = false;

void setup() {
    Serial.begin(115200);
    Serial.println("--- Smart Pole Fault Detection System ---");

    sensors.begin();
    comms.begin();
    safety.begin();
    
    Serial.println("System Initialized.");
}

void loop() {
    if (!safety.isCritical()) {
        // 1. Check Earthing
        float resistance = sensors.readEarthingResistance();
        if (resistance < 0) {
            Serial.println("FAULT: Broken Circuit Detected!");
            safety.setCritical(true);
        } else {
            Serial.printf("Earthing Resistance: %.2f Ohms\n", resistance);
            if (resistance > 3000) {
                Serial.println("FAULT: High Earthing Resistance!");
                safety.setCritical(true);
                comms.sendWhatsAppAlert("High Earthing Resistance", messageSent);
                comms.uploadData("001", false, true);
            }
        }

        // 2. Check Leakage
        double current = sensors.readLeakageCurrent();
        Serial.printf("Leakage Current: %.2f mA\n", current);
        if (current > 3.03) {
            Serial.println("FAULT: High Leakage Current!");
            safety.setCritical(true);
            comms.sendWhatsAppAlert("High Leakage Current", messageSent);
            comms.uploadData("001", true, false);
        }
    }

    // 3. Handle State & Reset
    safety.handleState();

    if (safety.isCritical()) {
        if (safety.checkReset()) {
            Serial.println("System Reset via Serial.");
            messageSent = false;
        }
    }

    delay(1000);
}
