#pragma once

#include <Arduino.h>
#include "../Protocol/Protocol.h"

class MotorController {
public:
    void begin(int m1a, int m1b, int m2a, int m2b,
               int m3a, int m3b, int m4a, int m4b);

    void execute(CommandType cmd);
    void stop();

private:
    int _pins[8];
    void setMotors(bool m1fwd, bool m2fwd, bool m3fwd, bool m4fwd);
};
