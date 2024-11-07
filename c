#include <Arduino.h>
#include "ESP32_Servo.h"
#include "HX711.h"
#include "DFRobotDFPlayerMini.h"

// 34 y 39 no se usan
#define boya 26
#define frecAgua 3000
#define pinServo 33
#define pinMotor 25
#define LED_BUILTIN 2

u_long t_ant = 0;
u_long t_ant2 = 0;
uint16_t frecuenciaPeso = 10000;
long peso = 0;
int comando = 0;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 35;
const int LOADCELL_SCK_PIN = 32;

DFRobotDFPlayerMini parlante;
HX711 balanza;
Servo compuerta;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup 1");
  Serial1.begin(9600);
  pinMode(boya, INPUT_PULLUP);
  pinMode(pinMotor, OUTPUT);
  Serial.println("Setup 3");
  parlante.begin(Serial1,0,0);
  Serial.println("INIT");
  parlante.volume(30);
  Serial.println("Ayrton 1");
  compuerta.attach(pinServo, 500, 2400);
  Serial.println("Ayrton 2");
  compuerta.write(0);
  Serial.println("Ayrton 4");
  balanza.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("Ayrton 5");
  balanza.tare(10); // Poner balanza en 0
  Serial.println("Ayrton 6");
  balanza.set_scale(-95); // Poner en gramos
  Serial.println("Ayrton 7");
  t_ant = millis();
}

void loop() {
  if (millis() > t_ant + frecAgua) {
    if (digitalRead(boya) == 0){
      Serial.print("Balanza ");
      Serial.println(1);
    } else {
      Serial.print("Balanza ");
      Serial.println(2);
    }
    t_ant = millis();
  }
  if (millis() > t_ant2 + frecuenciaPeso) {
    if (balanza.wait_ready_timeout(1000)) {
      peso = balanza.get_units(1);
      Serial.println(peso);
      if (peso < 30) {
        frecuenciaPeso = 2000;
        compuerta.write(90);
        digitalWrite(pinMotor, 1);
      } else if (peso > 200) {
        compuerta.write(0);
        digitalWrite(pinMotor, 0);
        frecuenciaPeso = 10000;
      }
    }
    t_ant2 = millis();
  }
  if (Serial.available()) {
    int in = Serial.parseInt();
    Serial.print("comando -> ");
    Serial.println(in);
    switch (in)
    {
    case 10:
      frecuenciaPeso = 2000;
      compuerta.write(90);
      digitalWrite(pinMotor, 1);
      break;
    case 81:
      Serial.println(in);
      parlante.play();
      break;
    case 82:
      Serial.println(in);
      parlante.pause();
      break;
    case 1:
      Serial.println(in);
      parlante.playFolder(1, 1);
      break;
    case 2:
      Serial.println(in);
      parlante.playFolder(1, 2);
      break;
    case 3:
      Serial.println(in);
      parlante.playFolder(1, 3);
      break;
    case 4:
      Serial.println(in);
      parlante.playFolder(1, 4);
      break;
    case 5:
      Serial.println(in);
      parlante.playFolder(1, 5);
      break;
    }
    in = 0;
  }
}



///////////////////////////////

; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino

monitor_speed = 115200
