// EspNowManager.h

#pragma once

#include <esp_now.h>
#include <WiFi.h>

class EspNowManager {
public:
    EspNowManager(uint8_t* peerAddress);

    bool begin();

    bool send(const uint8_t* data, size_t len);

private:
    uint8_t* _peerAddress;
};