#pragma once

#include <Arduino.h>
#include "../Protocol/Protocol.h"

class MotorController {
public:
    void begin(int m1a, int m1b, int m2a, int m2b,
               int m3a, int m3b, int m4a, int m4b);

    void execute(CommandType cmd, uint8_t speed = 255);
    void stop();

private:
    int _pins[8];
    int _channels[8];
    uint8_t _currentSpeed;
    void setMotors(int8_t m1, int8_t m2, int8_t m3, int8_t m4, uint8_t speed);
    void rampTo(int8_t m1, int8_t m2, int8_t m3, int8_t m4, uint8_t targetSpeed);
};
