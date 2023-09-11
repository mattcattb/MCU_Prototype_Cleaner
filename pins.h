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
const uint8_t SenseMR = 19; // Analog IS Current Sense for Right Motor
const uint8_t SenseML = 22; // Analog IS Current Sense for Left Motor

// LED Driver Pins (IS pin is set to high impedance).
const uint8_t MOSI_pin = 15; // IN0 LED activate output channel 0
const uint8_t MISO_pin = 16; // IN1 LED activate output channel 1
const uint8_t EN_LED_pin = 25; // DEN, “high”: diagnosis enabled. “low”: diagnosis disabled
const uint8_t SCK_pin = 15; // DESL Diagnosis Selection (DSEL) digital toggle between channels
const uint8_t SenseLED = 24; // IS Sense Output A/D analog digital converter pin
