#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include <functional>

using EspNowRecvCallback = std::function<void(const uint8_t* data, int len)>;

class EspNowManager {
public:
    EspNowManager(const uint8_t* peerAddress);

    bool begin();
    bool send(const uint8_t* data, size_t len);
    void onReceive(EspNowRecvCallback cb);

private:
    uint8_t _peerAddress[6];
    static EspNowRecvCallback _recvCallback;
    static void _rawRecvCallback(const uint8_t* mac,
                                 const uint8_t* data, int len);
};
