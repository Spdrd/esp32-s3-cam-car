#include <Arduino.h>

#include "GlobalConfig/GlobalConfig.h"
#include "Protocol/Protocol.h"
#include "EspNowManager/EspNowManager.h"
#include "CamManager/CamManager.h"
#include "MotorController/MotorController.h"

// MAC del controlador remoto — cambiar por la MAC real del otro ESP32
uint8_t controllerMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

EspNowManager espNow(controllerMac);
CamManager cam(&espNow);
MotorController motors;

volatile CommandType lastCommand = CMD_STOP;
volatile uint8_t lastSpeed = 255;
volatile bool newCommand = false;

void onDataReceived(const uint8_t* data, int len) {
    if (len == sizeof(ControlPacket)) {
        const ControlPacket* pkt = reinterpret_cast<const ControlPacket*>(data);
        if (pkt->magic == CONTROL_MAGIC) {
            lastCommand = pkt->command;
            lastSpeed = pkt->speed;
            newCommand = true;
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32-S3 Cam Car iniciando...");
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    motors.begin(M1_IN1, M1_IN2, M2_IN1, M2_IN2,
                 M3_IN1, M3_IN2, M4_IN1, M4_IN2);

    espNow.onReceive(onDataReceived);

    if (!espNow.begin()) {
        Serial.println("Fallo ESP-NOW");
        while (true) delay(1000);
    }

    if (!cam.begin()) {
        Serial.println("Fallo camara");
        while (true) delay(1000);
    }

    Serial.println("Todo listo. Esperando comandos...");
}

void loop() {
    if (newCommand) {
        newCommand = false;
        motors.execute(lastCommand, lastSpeed);
    }

    cam.sendFrame();
}
