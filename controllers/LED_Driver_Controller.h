#pragma once
#include <Arduino.h>
#include "pins.h"

class LED_Driver_Controller{

    unsigned long delay = 20; // how long to wait between LED behavior
    unsigned long prev_time;
    unsigned long cur_time;

    const double max_current_scaling = 1.5; // max amps for scaling IS voltage reading

    // LED channel outputs
    int ch_0;  
    int ch_1; 

    int IS_val; // IS current value

    void pin_setup();
    void LED_Driver_Controller_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val); // sends controls to LED

    void write_channels();

public:

    LED_Driver_Controller();
    void update(); // update based on time passed

     // get current sense for LED
    int get_IS();
    double get_current();
    double get_current(int num_avg);

    void set_channels(int ch_0, int ch_1); // set channels to on or off

};

LED_Driver_Controller::LED_Driver_Controller(){
  pin_setup();

  // start with all channels off
  this->ch_0 = 0;
  this->ch_1 = 0;

  this->prev_time = millis();
}

void LED_Driver_Controller::LED_Driver_Controller_Control(int in_0_val, int in_1_val, int DEN_val, int DSEL_val){ // sends controls to LED
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
  this->IS_val = analogRead(SenseLED_pin);

}

void LED_Driver_Controller::set_channels(int ch_0, int ch_1){
  // set channels to on or off
  this->ch_0 = ch_0;
  this->ch_1 = ch_1;
}

void LED_Driver_Controller::write_channels(){
  // write channels to LED
  LED_Driver_Controller_Control(this->ch_0, this->ch_1, 1, 1);
}

void LED_Driver_Controller::update(){

  this->cur_time = millis();

  // if enough time has passed, update LED
  if(cur_time - prev_time > delay){
    
    write_channels();
    prev_time = cur_time;
  }

}

int LED_Driver_Controller::get_IS(){ 
  // get current sense for LED
  return this->IS_val;
}

double LED_Driver_Controller::get_current(){
  // get IS value and scale according to max vlaue constant

  int IS = analogRead(SenseLED_pin);
  return IS* max_current_scaling; // upscale value
}


double LED_Driver_Controller::get_current(int num_avg){
  // get IS value and scale according to max vlaue constant

  double sum = 0;
  for(int i = 0; i < num_avg; i += 1){
    sum += get_current();
  }

  double avg = sum/num_avg;
  return avg;

}

void LED_Driver_Controller::pin_setup(){
    // led driver pin setup
    pinMode(MOSI_pin, OUTPUT);
    pinMode(MISO_pin, OUTPUT);
    pinMode(EN_LED_pin, OUTPUT);
    pinMode(SCK_pin, OUTPUT);

    pinMode(SenseLED_pin, INPUT);
}

