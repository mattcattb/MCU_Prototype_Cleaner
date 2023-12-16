#pragma once
#include <Arduino.h>
#include "../pins.h"

// simplified Vin_Convert struct for a more functional based programming view

struct Vin_Convert{

    double scaleM;
    double scaleB;

    double adc_reading; // latest adc 10bit value 
    double voltage_reading; // latest voltage value

    // initialization function
    Vin_Convert(double m = 1, double b = 0);
    void set_scaling(double m, double b);

    int analog_read(); // 0-1023V analog raw reading 
    double read(int n = 1); // reading average over n readings

    void quick_digital_write(const int pin, int value){
    // quickly does digital write on pin according to boolean value
        if(value == 1){
            digitalWrite(pin, HIGH); 
        }else if(value == 0){
            digitalWrite(pin, LOW);
        }
    }

};

