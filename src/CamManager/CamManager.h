// CameraManager.h

#pragma once

#include "esp_camera.h"
#include "../EspNowManager/EspNowManager.h"

class CamManager {
public:
    CamManager(EspNowManager* espNow);

    bool begin();
    void sendFrame();

private:
    EspNowManager* _espNow;
};