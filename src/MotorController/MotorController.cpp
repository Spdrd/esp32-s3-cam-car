#include "MotorController.h"
#include <driver/gpio.h>

void MotorController::begin(int m1a, int m1b, int m2a, int m2b,
                            int m3a, int m3b, int m4a, int m4b) {
    _pins[0] = m1a; _pins[1] = m1b;
    _pins[2] = m2a; _pins[3] = m2b;
    _pins[4] = m3a; _pins[5] = m3b;
    _pins[6] = m4a; _pins[7] = m4b;

    for (int i = 0; i < 8; i++) {
        gpio_reset_pin((gpio_num_t)_pins[i]);
        digitalWrite(_pins[i], LOW);
        pinMode(_pins[i], OUTPUT);
        digitalWrite(_pins[i], LOW);
    }
}

void MotorController::stop() {
    for (int i = 0; i < 8; i++) {
        digitalWrite(_pins[i], LOW);
    }
}

void MotorController::setMotors(bool m1fwd, bool m2fwd, bool m3fwd, bool m4fwd) {
    bool dirs[4] = {m1fwd, m2fwd, m3fwd, m4fwd};
    for (int i = 0; i < 4; i++) {
        digitalWrite(_pins[i * 2],     dirs[i] ? HIGH : LOW);
        digitalWrite(_pins[i * 2 + 1], dirs[i] ? LOW : HIGH);
    }
}

void MotorController::execute(CommandType cmd) {
    switch (cmd) {
        case CMD_FORWARD:
            setMotors(true, true, true, true);
            break;
        case CMD_BACKWARD:
            setMotors(false, false, false, false);
            break;
        case CMD_LEFT:
            // left motors backward, right motors forward
            setMotors(false, true, false, true);
            break;
        case CMD_RIGHT:
            // left motors forward, right motors backward
            setMotors(true, false, true, false);
            break;
        case CMD_STOP:
        default:
            stop();
            break;
    }
}
