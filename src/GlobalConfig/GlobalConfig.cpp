#include "GlobalConfig.h"

// --- BUTTON CONFIG ---
ButtonPinConfig buttonConfig = {
    BUTTON1_PIN,
    BUTTON2_PIN,
    BUTTON3_PIN
};

// --- SCREEN CONFIG ---
ScreenPinConfig screenConfig = {
    -1,
    -1,
    -1,
    TFT_DC,
    TFT_MOSI,
    TFT_SCLK
};

// --- MANAGE INSTANCES ---
BluetoothManager bleKeyboard;
ButtonManager buttons(buttonConfig);
ScreenManager screen(screenConfig);

// --- TEST CALLBACKS ---
void (*displayDaftPunk)() = []() {
        screen.showJpeg((uint8_t*)ovnitrix_jpg, ovnitrix_jpg_len, OVNNITRIX_WIDTH, OVNNITRIX_HEIGHT);
    };

// --- MUSIC CONTROLLER CALLBACKS
void (*playPauseCallback)() = []() {
        Serial.println("Play/Pause");
        screen.update("Play/Pause");
        bleKeyboard.sendConsumerControl(CC_PLAY_PAUSE);
    };

void (*nextTrackCallback)() = []() {
        Serial.println("Next Track");
        screen.update("Next Track");
        bleKeyboard.sendConsumerControl(CC_NEXT_TRACK);
        delay(BUTTON_COOLDOWN_MS);
    };

void (*previousTrackCallback)() = []() {
        Serial.println("Previous Track");
        screen.update("Previous Track");
        bleKeyboard.sendConsumerControl(CC_PREV_TRACK);
        delay(BUTTON_COOLDOWN_MS);
    };

void (*volumeUpCallback)() = []() {
        Serial.println("Volume Up");
        screen.update("Volume Up");
        bleKeyboard.sendConsumerControl(CC_VOLUME_UP);
        delay(BUTTON_COOLDOWN_MS);
    };

void (*volumeDownCallback)() = []() {
        Serial.println("Volume Down");
        screen.update("Volume Down");
        bleKeyboard.sendConsumerControl(CC_VOLUME_DOWN);
        delay(BUTTON_COOLDOWN_MS);
    };

ButtonActionCallbacks setupMusicControllerActions(){
    ButtonActionCallbacks buttonActions;
    buttonActions.onDoubleButton1Tap = playPauseCallback;
    buttonActions.onTripleTap = displayDaftPunk;
    buttonActions.onButton3 = nextTrackCallback;
    buttonActions.onButton2 = previousTrackCallback;
    buttonActions.onButton3_Button1 = volumeUpCallback;
    buttonActions.onButton2_Button1 = volumeDownCallback;
    return buttonActions;
}
