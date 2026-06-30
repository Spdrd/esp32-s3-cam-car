#include <Arduino.h>


#include "GlobalConfig/GlobalConfig.h"



void testMotors(int m1pin1, int m1pin2, int m2pin1, int m2pin2, int m3pin1, int m3pin2, int m4pin1, int m4pin2) {

  pinMode(m1pin1, OUTPUT);
  pinMode(m1pin2, OUTPUT);

  // Motor hacia adelante
  Serial.println("Adelante");
  digitalWrite(m1pin1, HIGH);
  digitalWrite(m1pin2, LOW);
  digitalWrite(m2pin1, HIGH);
  digitalWrite(m2pin2, LOW);
  digitalWrite(m3pin1, HIGH);
  digitalWrite(m3pin2, LOW);
  digitalWrite(m4pin1, HIGH);
  digitalWrite(m4pin2, LOW);
  delay(2000);

  // Frenar
  Serial.println("Stop");
  digitalWrite(m1pin1, LOW);
  digitalWrite(m1pin2, LOW);
  digitalWrite(m2pin1, LOW);
  digitalWrite(m2pin2, LOW);
  digitalWrite(m3pin1, LOW);
  digitalWrite(m3pin2, LOW);
  digitalWrite(m4pin1, LOW);
  digitalWrite(m4pin2, LOW);
  delay(1000);

  // Motor hacia atrás
  Serial.println("Atras");
  digitalWrite(m1pin1, LOW);
  digitalWrite(m1pin2, HIGH);
  digitalWrite(m2pin1, LOW);
  digitalWrite(m2pin2, HIGH);
  digitalWrite(m3pin1, LOW);
  digitalWrite(m3pin2, HIGH);
  digitalWrite(m4pin1, LOW);
  digitalWrite(m4pin2, HIGH);
  delay(2000);

  // Frenar
  Serial.println("Stop");
  digitalWrite(m1pin1, LOW);
  digitalWrite(m1pin2, LOW);
  digitalWrite(m2pin1, LOW);
  digitalWrite(m2pin2, LOW);
  digitalWrite(m3pin1, LOW);
  digitalWrite(m3pin2, LOW);
  digitalWrite(m4pin1, LOW);
  digitalWrite(m4pin2, LOW);
  delay(1000);
}

void setup() {

  Serial.begin(115200);
  delay(2000);

  Serial.println("Inicio");

  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT);
  pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT);
  pinMode(M4_IN2, OUTPUT);
  
}

void loop() {
  // testMotor(M1_IN1, M1_IN2);
  testMotors(M1_IN1, M1_IN2, M2_IN1, M2_IN2, M3_IN1, M3_IN2, M4_IN1, M4_IN2);
}