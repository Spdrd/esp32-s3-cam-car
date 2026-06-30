#include "EspNowManager.h"
#include <esp_wifi.h>

EspNowRecvCallback EspNowManager::_recvCallback = nullptr;

EspNowManager::EspNowManager(const uint8_t* peerAddress) {
    memcpy(_peerAddress, peerAddress, 6);
}

void EspNowManager::_rawRecvCallback(const uint8_t* mac,
                                     const uint8_t* data, int len) {
    if (_recvCallback) {
        _recvCallback(data, len);
    }
}

void EspNowManager::onReceive(EspNowRecvCallback cb) {
    _recvCallback = cb;
}

bool EspNowManager::begin() {
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error iniciando ESP-NOW");
        return false;
    }

    esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_1M_L);
    esp_now_register_recv_cb(_rawRecvCallback);

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
    return esp_now_send(_peerAddress, data, len) == ESP_OK;
}
