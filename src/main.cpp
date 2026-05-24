#include <Arduino.h>

// Pines Motores
#define M1_IN1 1
#define M1_IN2 2

void testMotor(int pin1, int pin2) {

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);

  // Motor hacia adelante
  Serial.println("Adelante");
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  delay(2000);

  // Frenar
  Serial.println("Stop");
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(1000);

  // Motor hacia atrás
  Serial.println("Atras");
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  delay(2000);

  // Frenar
  Serial.println("Stop");
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(1000);
}

void setup() {

  Serial.begin(115200);
  delay(2000);

  Serial.println("Inicio");

  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);

}

void loop() {
  testMotor(M1_IN1, M1_IN2);
}