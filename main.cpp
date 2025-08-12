//******************************************/
// Universidad del Valle de Guatemala
// BE3029 - Electronica Digital 2
// Juan Pablo Vivas
// 11/02/2025
// Laboratorio 4 DII parte C
// MCU: ESP32 dev kit 1.0
//******************************************/
// Librerias
//******************************************/
#include <Arduino.h>
#include <stdint.h>
//      Inclusión de libería para control de PWM
#include "driver/ledc.h"
//******************************************/
// Definiciones
//******************************************/
#define LEDr 21
#define LEDv 18
#define LEDa 5

#define Servo 13

#define btn1 33
#define btn2 23

#define delayBounce 250

#define canalR 0
#define canalV 1
#define canalA 2
#define canalServo 3

#define freqPWM 100
//    Frecuencia especial Servomotor
#define freqServo 50

#define resPWM 10
//    Alta resolución para mejor control de posición
#define resServo 12

//******************************************/
// Prototipos de funciones
//******************************************/
void initServo(void);
void initbtn(void);
void initleds(void);
void initPWM(void);
void IntensidadLEDs(uint16_t valor, uint8_t canal);
void CambioLED(uint16_t valor2);

// Prototipos ISRs
void IRAM_ATTR btn1_ISR(void);
void IRAM_ATTR btn2_ISR(void);

//******************************************/
// Variables globales
//******************************************/

volatile uint32_t cont1 = 0;
volatile uint32_t cont2 = 0;

volatile bool btn1Pressed;
volatile uint32_t lastISRbtn1 = 0;

volatile bool btn2Pressed;
volatile uint32_t lastISRbtn2 = 0;

//******************************************/
// ISRs Rutinas de Interrupcion
//******************************************/
void IRAM_ATTR btn1_ISR(void){
  uint32_t tiempoActual1 = millis();
  if (tiempoActual1 - lastISRbtn1 > delayBounce){
    cont1 = (cont1 + 1) % 4;
    btn1Pressed = true;
    lastISRbtn1 = tiempoActual1;
  } 
}

void IRAM_ATTR btn2_ISR(void){
  uint32_t tiempoActual2 = millis();
  if (tiempoActual2 - lastISRbtn2 > delayBounce){
    cont2 = (cont2 - 1 + 5) % 5;
    btn2Pressed = true;
    lastISRbtn2 = tiempoActual2;
  } 
}

//******************************************/
// Configuracion
//******************************************/
void setup() {
  initbtn();
  initleds();
  initPWM();
  initServo();
}

//******************************************/
// Loop Principal
//******************************************/
void loop() {
  if (btn1Pressed){
    btn1Pressed = false;
    CambioLED(cont1);
  }
  if (btn2Pressed){
    btn2Pressed = false;
    CambioLED(cont1);
  }
}
//******************************************/
// Otras funciones
//******************************************/
void initServo(void){
  //    Condiciones predeterminadas y calculadas para servomotor
  ledcSetup(canalServo, freqServo, resServo);

  ledcAttachPin(Servo, canalServo);
}
void initPWM(void){
  //    Asignar canales
  ledcSetup(canalR, freqPWM, resPWM);
  ledcSetup(canalV, freqPWM, resPWM);
  ledcSetup(canalA, freqPWM, resPWM);
  //    Asignar GPIO
  ledcAttachPin(LEDr, canalR);
  ledcAttachPin(LEDv, canalV);
  ledcAttachPin(LEDa, canalA);
}
void initbtn(void){
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  attachInterrupt(btn1, &btn1_ISR, FALLING);
  attachInterrupt(btn2, &btn2_ISR, FALLING);
}

void initleds(void){
  pinMode(LEDr, OUTPUT);
  pinMode(LEDv, OUTPUT);
  pinMode(LEDa, OUTPUT);

  digitalWrite(LEDr, LOW);
  digitalWrite(LEDv, LOW);
  digitalWrite(LEDa, LOW);
}

void IntensidadLEDs(uint16_t valor, uint8_t canal) {
  switch (valor) {
    case 0:
      ledcWrite(canal, 0);
      break;
    case 1:
      ledcWrite(canal, 220);
      break;
    case 2:
      ledcWrite(canal, 560);
      break;
    case 3:
      ledcWrite(canal, 990);
      break;
    case 4:
      ledcWrite(canal, 1023);
      break;
  }
}

void PosServo(uint16_t valorS, uint8_t canalS) {
  switch (valorS) {
    case 0:
      ledcWrite(canalS, 146); //    Prueba y error para rangos servomotor
      break;
    case 1:
      ledcWrite(canalS, 216);
      break;
    case 2:
      ledcWrite(canalS, 299);
      break;
    case 3:
      ledcWrite(canalS, 382);
      break;
    case 4:
      ledcWrite(canalS, 465);
      break;
  }
}

void CambioLED(uint16_t valor2){
  switch (valor2) {
    case 0:
      IntensidadLEDs(cont2, canalR);
      break;
    case 1:
      IntensidadLEDs(cont2, canalV);
      break;
    case 2:
      IntensidadLEDs(cont2, canalA);
      break;
    case 3:
      PosServo(cont2, canalServo); //    Implementación de rangos especiales para control de servo
      break;
  }
}

