#pragma once

#include <cstdint>

enum CommandType : uint8_t {
    CMD_STOP     = 0,
    CMD_FORWARD  = 1,
    CMD_BACKWARD = 2,
    CMD_LEFT     = 3,
    CMD_RIGHT    = 4,
};

struct __attribute__((packed)) ControlPacket {
    uint8_t magic;
    CommandType command;
    uint8_t speed;
};

const uint8_t CONTROL_MAGIC = 0xC1;

const uint8_t FRAME_END_MARKER[4] = {0xFF, 0xAA, 0xFF, 0xAA};

// ESP-NOW max payload = 250 bytes, leave room for header
const uint16_t VIDEO_CHUNK_SIZE = 240;
