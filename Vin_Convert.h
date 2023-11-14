#pragma once
#include <Arduino.h>
#include <cppQueue.h>
#include "pins.h"

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

Vin_Convert::Vin_Convert(double m, double b){
    pinMode(IN150V, INPUT);
    scaleM = m;
    scaleB = b;
}

void Vin_Convert::set_scaling(double m, double b){
    scaleM = m;
    scaleB = b;
}

int Vin_Convert::analog_read(){ 
    // 0-1023 basic analog read 
    adc_reading = analog_read(IN150V);
    
    return adc_reading;
}

double Vin_Convert::read(int n){ 
    // reading average over n readings
    double sum = 0;
    double volt; 
    for(int i = 0; i < n; i += 1){
        volt = ((double)analog_read())*5.0/1023.0;
        sum += (volt * scaleM + scaleB);
    }

    return sum/n;
}