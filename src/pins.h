#pragma once
#include <Arduino.h>

const uint8_t IN150V_pin = 23;

// unspecified pins 
const uint8_t AREF_pin = 20;
const uint8_t AVCC_pin = 18;

// Motor Driver Pins (IFX007T)
const uint8_t M_D_R_pin = 26; // input for right motor
const uint8_t M_D_L_pin = 27; // input for left motor
const uint8_t M_EN_pin = 28; // motor inhibit (set low to sleep)
const uint8_t SenseMR_pin = 19; // Analog IS Current Sense for Right Motor
const uint8_t SenseML_pin = 22; // Analog IS Current Sense for Left Motor

// LED Driver Pins (IS pin is set to high impedance).
const uint8_t IN0_LED_pin = 15; // IN0 LED activate output channel 0
const uint8_t IN1_LED_pin = 16; // IN1 LED activate output channel 1
const uint8_t EN_LED_pin = 25; // DEN, “high”: diagnosis enabled. “low”: diagnosis disabled
const uint8_t SCK_pin = 15; // DESL Diagnosis Selection (DSEL) digital toggle between channels
const uint8_t SenseLED_pin = 24; // IS Sense Output A/D analog digital converter pin

// Segment Displays
const uint8_t SEG1_pin = 12; 
const uint8_t SEG2_pin = 13;
const uint8_t SEG3_pin = 14;

// to segment display 
const uint8_t D_A_pin = 30;
const uint8_t D_B_pin = 31;
const uint8_t D_C_pin = 32;
const uint8_t D_D_pin = 1;
const uint8_t D_E_pin = 2;
const uint8_t D_F_pin = 9;
const uint8_t D_G_pin = 10;
const uint8_t D_dp_pin = 11;
