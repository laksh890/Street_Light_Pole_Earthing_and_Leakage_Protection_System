#include "SafetySystem.h"

SafetySystem::SafetySystem() : _critical(false) {}

void SafetySystem::begin() {
    pinMode(ALARM_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Default to isolated/safe state or connected?
    // In the original code, setup() does digitalWrite(RELAY_PIN, HIGH);
    // and handleCriticalState does digitalWrite(RELAY_PIN, HIGH) for critical.
    // Wait, the original code had:
    // setup: digitalWrite(RELAY_PIN, HIGH);
    // handleCriticalState:
    // if (critical) { digitalWrite(RELAY_PIN, HIGH); }
    // else { digitalWrite(RELAY_PIN, LOW); }
    // This implies HIGH is OFF/SAFE and LOW is ON/CONNECTED.
}

void SafetySystem::setCritical(bool state) {
    _critical = state;
}

bool SafetySystem::isCritical() {
    return _critical;
}

void SafetySystem::handleState() {
    if (_critical) {
        digitalWrite(ALARM_PIN, HIGH);
        digitalWrite(RELAY_PIN, HIGH);
    } else {
        digitalWrite(ALARM_PIN, LOW);
        digitalWrite(RELAY_PIN, LOW);
    }
}

bool SafetySystem::checkReset() {
    if (Serial.available() > 0) {
        char input = Serial.read();
        if (input == 'R' || input == 'r') {
            _critical = false;
            return true;
        }
    }
    return false;
}
