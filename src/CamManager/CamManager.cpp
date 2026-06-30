#include "CamManager.h"
#include "../GlobalConfig/GlobalConfig.h"

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

    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 10000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;

    if (psramFound()) {
        Serial.printf("PSRAM: %d bytes\n", ESP.getPsramSize());
        config.fb_count = 2;
        config.fb_location = CAMERA_FB_IN_PSRAM;
    } else {
        Serial.println("PSRAM no detectada, usando DRAM");
        config.fb_count = 1;
        config.fb_location = CAMERA_FB_IN_DRAM;
        config.frame_size = FRAMESIZE_QQVGA;
        config.jpeg_quality = 20;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Error camara: 0x%x\n", err);
        return false;
    }

    sensor_t* s = esp_camera_sensor_get();
    s->set_hmirror(s, 1);

    Serial.println("Camara lista");
    return true;
}

void CamManager::sendFrame() {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Error capturando frame");
        return;
    }

    for (size_t i = 0; i < fb->len; i += VIDEO_CHUNK_SIZE) {
        size_t currentSize = (fb->len - i < VIDEO_CHUNK_SIZE)
            ? fb->len - i
            : VIDEO_CHUNK_SIZE;

        _espNow->send(fb->buf + i, currentSize);
        delay(1);
    }

    _espNow->send(FRAME_END_MARKER, 4);
    esp_camera_fb_return(fb);
}
