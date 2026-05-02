#include "SensorManager.h"

SensorManager::SensorManager() {}

void SensorManager::begin() {
    pinMode(V2_PIN, INPUT);
    emon1.current(CURRENT_SENSOR_PIN, CURRENT_CALIBRATION);
}

double SensorManager::readLeakageCurrent() {
    double Irms = 0;
    // Discard first reading for stabilization
    emon1.calcIrms(1480);
    delay(10);

    for (int i = 0; i < 4; i++) {
        Irms += emon1.calcIrms(1480) * 100;
        delay(10);
    }
    return Irms / 4;
}

float SensorManager::readEarthingResistance() {
    int analog_value = analogRead(V2_PIN);
    float v2 = (analog_value / 4095.0) * 3.3;

    if (isBrokenCircuit(v2)) {
        return -1.0f; // Indicator for broken circuit
    }

    float resistance = v2 / ((V1_VOLTAGE - v2) / R1_RESISTANCE);
    return resistance;
}

bool SensorManager::isBrokenCircuit(float v2) {
    return abs(V1_VOLTAGE - v2) < 0.01;
}
