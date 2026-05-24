// EspNowManager.cpp

#include "EspNowManager.h"

EspNowManager::EspNowManager(uint8_t* peerAddress) {
    _peerAddress = peerAddress;
}

bool EspNowManager::begin() {

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error iniciando ESP-NOW");
        return false;
    }

    esp_now_peer_info_t peerInfo = {};

    memcpy(peerInfo.peer_addr, _peerAddress, 6);

    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Error agregando peer");
        return false;
    }

    Serial.println("ESP-NOW listo");

    return true;
}

bool EspNowManager::send(const uint8_t* data, size_t len) {

    esp_err_t result = esp_now_send(
        _peerAddress,
        data,
        len
    );

    return result == ESP_OK;
}