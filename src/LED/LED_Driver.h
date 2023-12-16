#pragma once
#include <Arduino.h>
#include "../pins.h"

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

    void quick_digital_write(const int pin, int value){
    // quickly does digital write on pin according to boolean value
        if(value == 1){
            digitalWrite(pin, HIGH); 
        }else if(value == 0){
            digitalWrite(pin, LOW);
        }
    }

};

