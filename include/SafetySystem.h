#ifndef SAFETY_SYSTEM_H
#define SAFETY_SYSTEM_H

#include <Arduino.h>
#include "config.h"

class SafetySystem {
public:
    SafetySystem();
    void begin();
    void setCritical(bool state);
    bool isCritical();
    void handleState();
    bool checkReset();

private:
    bool _critical;
};

#endif
