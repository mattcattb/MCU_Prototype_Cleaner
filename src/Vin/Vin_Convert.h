#pragma once
#include <Arduino.h>
#include "../pins.h"

// simplified Vin_Convert struct for a more functional based programming view

struct Vin_Convert{

    double scaleM;
    double scaleB;

    double adc_reading; // latest adc 10bit value 
    double voltage_reading; // latest voltage value

    Vin_Convert(double m = 1, double b = 0);
    
    void set_scaling(double m, double b);
    int analog_read(); // 0-1023V analog read 

    double read(int n = 1); // reading average over n readings

};
