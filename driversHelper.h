#pragma once
#include <Arduino.h>

#include "generalHelpers.h"

namespace motor{

// Motor Driver Pins (IFX007T)
const uint8_t M_D_R = 26; // input for right motor
const uint8_t M_D_L = 27; // input for left motor
const uint8_t M_EN = 28; // motor inhibit (set low to sleep)
const uint8_t SenseMR = 19; // Analog IS Current Sense for Right Motor
const uint8_t SenseML = 22; // Analog IS Current Sense for Left Motor

unsigned long reading_delay = 20; // how long to wait between readings
unsigned long prev_time = 0;
unsigned long cur_time;

void Motor_Driver_Control(int in_L, int in_R, int INH, int *L_Sense, int *R_Sense){
  /*
   * in_L: input for left motor (High or Low)
   * int_R: input for right motor
   * INH: inhibit, set low to sleep, high to allow motor use (M_EN)
   * L_Sense: Current Sense Left Driver (0-1023)
   * R_Sense: Current Sense Right Driver (0-1023)
   */

  // set left motor
  gen::quick_digital_write(M_D_L, in_L);

  // set right motor
  gen::quick_digital_write(M_D_R, in_R);

  // set inhibit
  gen::quick_digital_write(M_EN, INH);

  *L_Sense = analogRead(SenseML);
  *R_Sense = analogRead(SenseMR); 

}

void update_motor(){

    // update motor if enough time has passed

    cur_time = millis();

    if (cur_time - prev_time > reading_delay){
        //! update motor!
        prev_time = cur_time;
    }

}

void pin_setup(){
    // motor driver pin setup
    pinMode(M_D_R, OUTPUT);
    pinMode(M_D_L, OUTPUT);
    pinMode(M_EN, OUTPUT);
    pinMode(SenseMR, INPUT);
    pinMode(SenseML, INPUT);

}

}

namespace led{

void LED_Driver_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val, int *IS); // sends controls to LED
void update_led();
void pin_setup();

// LED Driver Pins (IS pin is set to high impedance).
const uint8_t MOSI_pin = 15; // IN0 LED activate output channel 0
const uint8_t MISO_pin = 16; // IN1 LED activate output channel 1
const uint8_t EN_LED_pin = 25; // DEN, “high”: diagnosis enabled. “low”: diagnosis disabled
const uint8_t SCK_pin = 15; // DESL Diagnosis Selection (DSEL) digital toggle between channels
const uint8_t SenseLED = 24; // IS Sense Output A/D analog digital converter pin

unsigned long delay = 20; // how long to wait between LED behavior
unsigned long prev_time = 0;
unsigned long cur_time;

void LED_Driver_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val, int *IS){ // sends controls to LED
  /*
   * in_0: signal for channel 0
   * int_1: signal for channel 1
   * DEN: Diagnostic Enabler, Enabler for LED
   * DSEL: Diagnosis Selection (SCK_pin)
   * IS: Current Sense for LED (0-1023)
   */

  // set channel 0 LED
  gen::quick_digital_write(MOSI_pin, in_0_val);

  // set channel 1 LED
  gen::quick_digital_write(MISO_pin, in_1_val);

  // set Diagnostic Enabler
  gen::quick_digital_write(EN_LED_pin, DEN_val);
  
  // set Diagnosis Selection
  gen::quick_digital_write(SCK_pin, DSEL_val);

  // read Current Sense for LED and save in IS pointer
  *IS = analogRead(SenseLED);

}

void update_led(){

  // if enough time has passed, update LED
  if(cur_time - prev_time > delay){
    // update LED
    prev_time = cur_time;
  }

}

void pin_setup(){
    // led driver pin setup
    pinMode(MOSI_pin, OUTPUT);
    pinMode(MISO_pin, OUTPUT);
    pinMode(EN_LED_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);

    pinMode(SenseLED, INPUT);
}


}
