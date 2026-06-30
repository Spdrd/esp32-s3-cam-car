#include "MotorController.h"
#include <driver/gpio.h>

const int PWM_FREQ = 1000;
const int PWM_RESOLUTION = 8; // 0-255

void MotorController::begin(int m1a, int m1b, int m2a, int m2b,
                            int m3a, int m3b, int m4a, int m4b) {
    _pins[0] = m1a; _pins[1] = m1b;
    _pins[2] = m2a; _pins[3] = m2b;
    _pins[4] = m3a; _pins[5] = m3b;
    _pins[6] = m4a; _pins[7] = m4b;
    _currentSpeed = 0;

    for (int i = 0; i < 8; i++) {
        gpio_reset_pin((gpio_num_t)_pins[i]);
        ledcSetup(i, PWM_FREQ, PWM_RESOLUTION);
        ledcAttachPin(_pins[i], i);
        ledcWrite(i, 0);
    }
}

void MotorController::stop() {
    for (int i = 0; i < 8; i++) {
        ledcWrite(i, 0);
    }
    _currentSpeed = 0;
}

// dir: 1 = forward, -1 = backward, 0 = stopped
void MotorController::setMotors(int8_t m1, int8_t m2, int8_t m3, int8_t m4, uint8_t speed) {
    int8_t dirs[4] = {m1, m2, m3, m4};
    for (int i = 0; i < 4; i++) {
        int chA = i * 2;
        int chB = i * 2 + 1;
        if (dirs[i] > 0) {
            ledcWrite(chA, speed);
            ledcWrite(chB, 0);
        } else if (dirs[i] < 0) {
            ledcWrite(chA, 0);
            ledcWrite(chB, speed);
        } else {
            ledcWrite(chA, 0);
            ledcWrite(chB, 0);
        }
    }
    _currentSpeed = speed;
}

void MotorController::rampTo(int8_t m1, int8_t m2, int8_t m3, int8_t m4, uint8_t targetSpeed) {
    for (int s = 80; s < targetSpeed; s += 20) {
        setMotors(m1, m2, m3, m4, (uint8_t)s);
        delay(15);
    }
    setMotors(m1, m2, m3, m4, targetSpeed);
}

void MotorController::execute(CommandType cmd, uint8_t speed) {
    switch (cmd) {
        case CMD_FORWARD:
            rampTo(1, 1, 1, 1, speed);
            break;
        case CMD_BACKWARD:
            rampTo(-1, -1, -1, -1, speed);
            break;
        case CMD_LEFT:
            rampTo(-1, 1, -1, 1, speed);
            break;
        case CMD_RIGHT:
            rampTo(1, -1, 1, -1, speed);
            break;
        case CMD_STOP:
        default:
            stop();
            break;
    }
}
