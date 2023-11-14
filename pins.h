#pragma once
#include <Arduino.h>

const uint8_t IN150V = 23;

// unspecified pins 
const uint8_t AREF = 20;
const uint8_t AVCC = 18;

// Motor Driver Pins (IFX007T)
const uint8_t M_D_R = 26; // input for right motor
const uint8_t M_D_L = 27; // input for left motor
const uint8_t M_EN = 28; // motor inhibit (set low to sleep)
const uint8_t SenseMR_pin = 19; // Analog IS Current Sense for Right Motor
const uint8_t SenseML_pin = 22; // Analog IS Current Sense for Left Motor

// LED Driver Pins (IS pin is set to high impedance).
const uint8_t MOSI_pin = 15; // IN0 LED activate output channel 0
const uint8_t MISO_pin = 16; // IN1 LED activate output channel 1
const uint8_t EN_LED_pin = 25; // DEN, “high”: diagnosis enabled. “low”: diagnosis disabled
const uint8_t SCK_pin = 15; // DESL Diagnosis Selection (DSEL) digital toggle between channels
const uint8_t SenseLED_pin = 24; // IS Sense Output A/D analog digital converter pin

// Segment Displays
const uint8_t SEG1 = 12; 
const uint8_t SEG2 = 13;
const uint8_t SEG3 = 14;

// to segment display 
const uint8_t D_A = 30;
const uint8_t D_B = 31;
const uint8_t D_C = 32;
const uint8_t D_D = 1;
const uint8_t D_E = 2;
const uint8_t D_F = 9;
const uint8_t D_G = 10;
const uint8_t D_dp = 11;

// number of digits for precision
uint8_t precision = 4; // 8 units of precision 

void quick_digital_write(int pin, int value){
// quickly does digital write on pin according to boolean value
    if(value == 1){
        digitalWrite(pin, HIGH); 
    }else if(value == 0){
        digitalWrite(pin, LOW);
    }
}

int pack_double(double val){
  // packs double into an integer according to set precision
  
  
}
