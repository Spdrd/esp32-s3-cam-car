// CameraManager.cpp

#include "CamManager.h"

// Pines para ESP32-S3 WROOM Cam
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    15
#define SIOD_GPIO_NUM    4
#define SIOC_GPIO_NUM    5
#define Y9_GPIO_NUM      16
#define Y8_GPIO_NUM      17
#define Y7_GPIO_NUM      18
#define Y6_GPIO_NUM      12
#define Y5_GPIO_NUM      10
#define Y4_GPIO_NUM      8
#define Y3_GPIO_NUM      9
#define Y2_GPIO_NUM      11
#define VSYNC_GPIO_NUM   6
#define HREF_GPIO_NUM    7
#define PCLK_GPIO_NUM    13

CamManager::CamManager(EspNowManager* espNow) {
    _espNow = espNow;
}

bool CamManager::begin() {

    camera_config_t config;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 10000000;

    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;

    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    esp_err_t err = esp_camera_init(&config);

    if (err != ESP_OK) {

        Serial.printf(
            "Error cámara: 0x%x\n",
            err
        );

        return false;
    }

    Serial.println("Camara lista");

    return true;
}

void CamManager::sendFrame() {

    camera_fb_t* fb = esp_camera_fb_get();

    if (!fb) {
        Serial.println("Error capturando frame");
        return;
    }

    const uint16_t chunkSize = 240;

    for (size_t i = 0; i < fb->len; i += chunkSize) {

        size_t currentSize =
            (fb->len - i < chunkSize)
            ? fb->len - i
            : chunkSize;

        _espNow->send(
            fb->buf + i,
            currentSize
        );

        delay(1);
    }

    // Señal fin frame
    uint8_t endSig[4] = {
        0xFF,
        0xAA,
        0xFF,
        0xAA
    };

    _espNow->send(endSig, 4);

    esp_camera_fb_return(fb);
}