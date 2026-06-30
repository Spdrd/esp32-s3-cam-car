# ESP32-S3 Cam Car

<img width="1200" height="1600" alt="image" src="https://github.com/user-attachments/assets/ae6310e9-1694-48f6-8a20-8580d80cb28e" />


Vehículo controlado remotamente con cámara en tiempo real, usando ESP-NOW como protocolo de comunicación inalámbrica.

## Hardware

| Componente | Detalle |
|---|---|
| Microcontrolador | ESP32-S3 WROOM (sin PSRAM) |
| Cámara | OV2640 (conector FPC) |
| Driver de motores | 2x DRV8833 |
| Motores | 4 motores DC (tracción 4WD) |
| Batería | Li-ion |

### Pinout

**Motores:**

| Motor | Posición | IN1 | IN2 |
|---|---|---|---|
| M1 | Trasero izquierdo | GPIO 1 | GPIO 2 |
| M2 | Trasero derecho | GPIO 3 | GPIO 14 |
| M3 | Delantero izquierdo | GPIO 21 | GPIO 47 |
| M4 | Delantero derecho | GPIO 48 | GPIO 35 |

**Cámara (OV2640):**

| Señal | GPIO |
|---|---|
| XCLK | 15 |
| SDA | 4 |
| SCL | 5 |
| VSYNC | 6 |
| HREF | 7 |
| PCLK | 13 |
| D0-D7 | 11, 9, 8, 10, 12, 18, 17, 16 |

## Comunicación

Usa **ESP-NOW** para comunicación bidireccional con un controlador remoto (otro ESP32):

- **Vehículo → Controlador:** Stream de video JPEG (QQVGA 160x120, chunks de 240 bytes)
- **Controlador → Vehículo:** Comandos de movimiento (forward, backward, left, right, stop)
- **Data rate:** 1 Mbps (largo alcance)
- **MAC del vehículo:** `80:B5:4E:C6:F9:1C`

### Protocolo

Definido en `src/Protocol/Protocol.h` — debe copiarse al proyecto del controlador.

**Paquete de control (3 bytes):**

```
| magic (0xC1) | command | speed (0-255) |
```

Comandos: `CMD_STOP=0`, `CMD_FORWARD=1`, `CMD_BACKWARD=2`, `CMD_LEFT=3`, `CMD_RIGHT=4`

**Stream de video:**

Frames JPEG fragmentados en chunks de 240 bytes. Fin de frame marcado con `{0xFF, 0xAA, 0xFF, 0xAA}`.

## Estructura del proyecto

```
src/
├── main.cpp                 # Setup y loop principal
├── Protocol/Protocol.h      # Protocolo compartido (copiar al controlador)
├── GlobalConfig/            # Definiciones de pines
├── EspNowManager/           # Gestión ESP-NOW (envío + recepción)
├── CamManager/              # Captura y streaming de cámara
└── MotorController/         # Control PWM de 4 motores con rampa de arranque
```

## Build

Requiere [PlatformIO](https://platformio.org/).

```bash
# Compilar
pio run

# Flashear
pio run -t upload

# Monitor serial
pio device monitor
```

## Notas

- Los motores usan PWM (1kHz, 8 bits) con rampa de arranque para evitar picos de corriente
- La cámara opera sin PSRAM (1 frame buffer en DRAM)
- Ver `REPORTE_ESPNOW.md` para el detalle completo del protocolo y guía de implementación del controlador remoto
