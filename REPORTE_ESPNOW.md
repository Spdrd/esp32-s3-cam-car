# Reporte: ESP32-S3 Cam Car — Protocolo ESP-NOW

## Resumen

El ESP32-S3 (vehículo) está configurado para:
1. **Recibir** comandos de control de motores via ESP-NOW
2. **Enviar** frames JPEG de la cámara via ESP-NOW (streaming)

El agente receptor debe implementar el **controlador remoto**: un segundo ESP32 que envía comandos y recibe/muestra el video.

---

## Arquitectura

```
[Controlador remoto (ESP32)]  <-- ESP-NOW -->  [Vehículo (ESP32-S3)]
     Envía: ControlPacket                        Recibe: ControlPacket → mueve motores
     Recibe: chunks JPEG                         Envía: chunks JPEG de cámara
```

Ambos dispositivos son peers ESP-NOW mutuos. La comunicación es bidireccional sobre el mismo canal (0), sin encriptación.

---

## Protocolo de comunicación

### 1. Paquete de control (Controlador → Vehículo)

Definido en `src/Protocol/Protocol.h` — **este archivo es compartido, debe copiarse al proyecto del controlador**.

```cpp
enum CommandType : uint8_t {
    CMD_STOP     = 0,
    CMD_FORWARD  = 1,
    CMD_BACKWARD = 2,
    CMD_LEFT     = 3,
    CMD_RIGHT    = 4,
};

struct __attribute__((packed)) ControlPacket {
    uint8_t magic;        // siempre 0xC1
    CommandType command;  // el comando
    uint8_t speed;        // 0-255 (reservado para PWM futuro, actualmente no usado)
};

const uint8_t CONTROL_MAGIC = 0xC1;
```

**Tamaño:** 3 bytes  
**Magic byte:** `0xC1` — el vehículo descarta paquetes sin este magic  
**Envío:** El controlador debe enviar un `ControlPacket` cada vez que cambie el estado del joystick/botones

### 2. Stream de video (Vehículo → Controlador)

El vehículo envía frames JPEG fragmentados:

- **Formato de imagen:** JPEG, resolución QQVGA (160x120), calidad 12
- **Fragmentación:** Cada frame se divide en chunks de máximo **240 bytes** (ESP-NOW permite 250 max)
- **Marcador de fin de frame:** 4 bytes `{0xFF, 0xAA, 0xFF, 0xAA}`
- **Delay entre chunks:** 1ms

**Flujo de recepción en el controlador:**
```
1. Recibir paquete ESP-NOW
2. Si paquete == {0xFF, 0xAA, 0xFF, 0xAA} (4 bytes exactos):
     → El buffer acumulado es un frame JPEG completo
     → Procesarlo/mostrarlo
     → Limpiar buffer
3. Si no:
     → Acumular los bytes en un buffer
```

**Tamaño típico de un frame JPEG a QQVGA q12:** ~3-8 KB  
**Chunks por frame:** ~15-35  
**FPS estimado:** ~5-10 fps (limitado por ESP-NOW throughput)

---

## Información de red

### Vehículo (ESP32-S3)
- **WiFi mode:** `WIFI_STA`
- **Canal ESP-NOW:** 0
- **Encriptación:** deshabilitada
- **MAC address:** Se imprime por serial al iniciar (`Serial.println(WiFi.macAddress())`)
- **MAC del peer (controlador):** Configurada en `src/main.cpp` línea 9:
  ```cpp
  uint8_t controllerMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  ```
  **IMPORTANTE:** Debe cambiarse por la MAC real del controlador remoto.

### Controlador remoto (a implementar)
- **WiFi mode:** debe ser `WIFI_STA`
- **Canal ESP-NOW:** 0
- **Peer address:** la MAC del ESP32-S3 del vehículo (obtenida del serial)

---

## Qué debe implementar el controlador remoto

### Hardware sugerido
- ESP32 (cualquier variante con WiFi)
- Pantalla (SPI TFT recomendado: ST7789, ILI9341, o similar) para mostrar video
- Joystick analógico o botones para dirección

### Software necesario

1. **Inicializar ESP-NOW** en modo `WIFI_STA`, agregar el peer (MAC del vehículo)

2. **Enviar comandos:** Leer joystick/botones, construir un `ControlPacket` y enviarlo:
   ```cpp
   ControlPacket pkt;
   pkt.magic = CONTROL_MAGIC;  // 0xC1
   pkt.command = CMD_FORWARD;  // o CMD_LEFT, CMD_RIGHT, etc.
   pkt.speed = 255;            // reservado
   esp_now_send(vehicleMac, (uint8_t*)&pkt, sizeof(pkt));
   ```

3. **Recibir video:** Registrar callback de recepción ESP-NOW:
   ```cpp
   // Buffer global
   uint8_t jpegBuffer[20000];
   size_t jpegLen = 0;
   volatile bool frameReady = false;

   void onReceive(const uint8_t* mac, const uint8_t* data, int len) {
       // Detectar fin de frame
       if (len == 4 && data[0] == 0xFF && data[1] == 0xAA
                    && data[2] == 0xFF && data[3] == 0xAA) {
           frameReady = true;
           return;
       }
       // Acumular chunks
       if (jpegLen + len < sizeof(jpegBuffer)) {
           memcpy(jpegBuffer + jpegLen, data, len);
           jpegLen += len;
       }
   }
   ```

4. **Mostrar video:** En el loop, cuando `frameReady == true`:
   - Decodificar JPEG desde `jpegBuffer` (tamaño `jpegLen`)
   - Dibujar en la pantalla TFT (usando TJpgDec o similar)
   - Resetear: `jpegLen = 0; frameReady = false;`

5. **Opción sin pantalla:** Si no hay pantalla, el controlador puede reenviar los frames JPEG por Serial a un PC, donde un script Python los muestre:
   ```python
   # Ejemplo Python receptor serial
   import serial, cv2, numpy as np
   ser = serial.Serial('COM3', 921600)
   while True:
       # Leer largo del frame (2 bytes little-endian)
       size = int.from_bytes(ser.read(2), 'little')
       data = ser.read(size)
       img = cv2.imdecode(np.frombuffer(data, np.uint8), cv2.IMREAD_COLOR)
       if img is not None:
           cv2.imshow('Cam', img)
           cv2.waitKey(1)
   ```
   (El firmware del controlador enviaría: `Serial.write((uint8_t*)&jpegLen, 2); Serial.write(jpegBuffer, jpegLen);`)

---

## Estructura del proyecto del vehículo

```
src/
├── main.cpp                    # Setup y loop principal
├── Protocol/
│   └── Protocol.h              # ★ Definiciones compartidas (copiar al controlador)
├── GlobalConfig/
│   ├── GlobalConfig.h          # Pines de cámara y motores
│   └── GlobalConfig.cpp
├── EspNowManager/
│   ├── EspNowManager.h         # Clase ESP-NOW con send + receive callback
│   └── EspNowManager.cpp
├── CamManager/
│   ├── CamManager.h            # Captura y envío de frames JPEG
│   └── CamManager.cpp
└── MotorController/
    ├── MotorController.h       # Control de 4 motores DC
    └── MotorController.cpp
```

---

## Pines del vehículo (referencia)

### Motores (4 motores DC, cada uno con 2 pines H-bridge)
| Motor | Función | IN1 | IN2 |
|-------|---------|-----|-----|
| M1 | Trasero izquierdo | GPIO 1 | GPIO 2 |
| M2 | Trasero derecho | GPIO 3 | GPIO 14 |
| M3 | Delantero izquierdo | GPIO 21 | GPIO 47 |
| M4 | Delantero derecho | GPIO 48 | GPIO 35 |

### Cámara (OV2640 en ESP32-S3)
Pines definidos en `GlobalConfig.h` — XCLK=15, SDA=4, SCL=5, VSYNC=6, HREF=7, PCLK=13, D0-D7 en varios GPIOs.

---

## Limitaciones y notas

- **ESP-NOW max payload:** 250 bytes. Los chunks de video son de 240 bytes para dejar margen.
- **No hay ACK de video:** si un chunk se pierde, el frame se corrompe. JPEG es robusto a corrupción parcial pero puede haber artefactos.
- **Latencia:** ~100-200ms entre captura y recepción.
- **Alcance:** ~100-200m en línea de vista (depende de antena).
- **El campo `speed` en ControlPacket** está reservado para PWM futuro. Actualmente los motores operan a velocidad máxima (on/off con digitalWrite).
- **Dirección de giro:** LEFT = motores izquierdos atrás + derechos adelante (giro en sitio). RIGHT = inverso.
- **La MAC del controlador** está en `src/main.cpp` como `0xFF,0xFF,0xFF,0xFF,0xFF,0xFF` (broadcast). Debe cambiarse por la MAC real para comunicación punto a punto.

---

## Pasos para poner en marcha

1. Flashear el vehículo con `pio run -t upload`
2. Abrir el monitor serial → anotar la MAC address que imprime
3. En el controlador remoto: usar esa MAC como peer address
4. En `src/main.cpp` del vehículo: cambiar `controllerMac[]` por la MAC del controlador
5. Reflashear el vehículo
6. Ambos dispositivos se comunican automáticamente al encender
