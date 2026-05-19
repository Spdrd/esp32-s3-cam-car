#include <Arduino.h>

// Pines Motores
#define M1_IN1 1
#define M1_IN2 2

#define M2_IN1 3
#define M2_IN2 14

#define M3_IN1 21
#define M3_IN2 47

#define M4_IN1 48
#define M4_IN2 35

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
  testMotor(M1_IN1, M1_IN2);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}