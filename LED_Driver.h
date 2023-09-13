#pragma once
#include <Arduino.h>
#include "pins.h"

class LED_Driver{

    unsigned long delay = 20; // how long to wait between LED behavior
    unsigned long prev_time = 0;
    unsigned long cur_time;

    void pin_setup();

public:

    LED_Driver();
    
    void LED_Driver_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val, int *IS); // sends controls to LED
    void update();

};

LED_Driver::LED_Driver(){
  pin_setup();
}

void LED_Driver::LED_Driver_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val, int *IS){ // sends controls to LED
  /*
   * in_0: signal for channel 0
   * int_1: signal for channel 1
   * DEN: Diagnostic Enabler, Enabler for LED
   * DSEL: Diagnosis Selection (SCK_pin)
   * IS: Current Sense for LED (0-1023)
   */

  // set channel 0 LED
  quick_digital_write(MOSI_pin, in_0_val);

  // set channel 1 LED
  quick_digital_write(MISO_pin, in_1_val);

  // set Diagnostic Enabler
  quick_digital_write(EN_LED_pin, DEN_val);
  
  // set Diagnosis Selection
  quick_digital_write(SCK_pin, DSEL_val);

  // read Current Sense for LED and save in IS pointer
  *IS = analogRead(SenseLED);

}

void LED_Driver::update(){

  static unsigned long prev_time = millis();
  unsigned long cur_time = millis();

  // if enough time has passed, update LED
  if(cur_time - prev_time > delay){
    // update LED
    prev_time = cur_time;
  }

}

void LED_Driver::pin_setup(){
    // led driver pin setup
    pinMode(MOSI_pin, OUTPUT);
    pinMode(MISO_pin, OUTPUT);
    pinMode(EN_LED_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);

    pinMode(SenseLED, INPUT);
}
