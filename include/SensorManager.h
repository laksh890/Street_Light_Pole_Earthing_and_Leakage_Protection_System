#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "EmonLib.h"
#include "config.h"

class SensorManager {
public:
    SensorManager();
    void begin();
    double readLeakageCurrent();
    float readEarthingResistance();
    bool isBrokenCircuit(float v2);

private:
    EnergyMonitor emon1;
};

#endif
