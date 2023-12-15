#pragma once
#include <Arduino.h>
#include "pins.h"

struct LED_Driver
{
    double sense_scaling_m;
    double sense_scaling_b;

    int ch_0_state;
    int ch_1_state;

    double current; // latest current value 
    
    void LED_Driver_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val);

    void set_scaling(double m, double b);

    void pin_setup();
    LED_Driver();

    void write(int ch_0, int ch_1); // set LEDL, LEDR channels
    void write(); // uses the preset states 

    double read(int n = 1); // get current reading amps

};

LED_Driver::LED_Driver(){
  pin_setup();

  // start with all channels off
  this->ch_0_state = 0;
  this->ch_1_state = 0;
}

void LED_Driver::set_scaling(double m, double b){
    // set scaling values for current
    sense_scaling_m = m;
    sense_scaling_b = b;
}

void LED_Driver::pin_setup(){
    // led driver pin setup
    pinMode(MOSI_pin, OUTPUT);
    pinMode(MISO_pin, OUTPUT);
    pinMode(EN_LED_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);

    pinMode(SenseLED_pin, INPUT);
}


void LED_Driver::LED_Driver_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val){ // sends controls to LED
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
  this->current = analogRead(SenseLED_pin);

}

void LED_Driver::write(){ 
    // set LEDL, LEDR channels using preset ch states
    LED_Driver_Control(this->ch_0_state, this->ch_1_state, 1, 1);
}

void LED_Driver::write(int ch_0, int ch_1){ 
    // set LEDL, LEDR channels using current ch states
    LED_Driver_Control(ch_0, ch_1, 1, 1);
}

double LED_Driver::read(int n){
    // scale to get reading in amps over int n 
    double sum = 0;
    int analog_val;
    for(int i = 0; i < n; i += 1){
        analog_val = analogRead(SenseLED_pin);
        sum += analog_val*sense_scaling_m + sense_scaling_b;
    }

    return sum/n;
}
