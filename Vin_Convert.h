#pragma once
#include <Arduino.h>
#include "pins.h"

//! NEED TO FIX FUCKED UP THINGS READING!!

class Vin_Convert{

    const uint8_t in_pin;

    int scaleM;
    int scaleB;

    // pointer to reading
    double *reading_ptr;
    
    // actual readings
    double reading; 
    double reading_scaled

    unsigned long prev_time; 
    unsigned long cur_time;
    unsigned long reading_delay;

    // take reading from Vin
    double volt_read();
    double volt_scaled_read();

public:

    Vin_Convert();

    // update reading if ms has passed
    void update_reading();

    void set_reading_ptr(double *reading);

    // get already stored reading
    double get_stored_reading();
    double get_stored_scaled_reading();

    // set scaling factor
    void set_scaling(double m, double b);
};

Vin_Convert::Vin_Convert(){

    // set timing
    this->prev_time = millis();
    this->reading_delay = 20;

    // set input pins
    pinMode(IN150V, INPUT);

    // take 1 reading to initialize
    volt_scaled_read();

}


void Vin_Convert::update_reading(){
    // update reading if enough time has passed

    this->cur_time = millis();

    // possibly update reading from 150V input
    if(this->cur_time - this->prev_time > this->reading_delay){
        store_volt_scaled_read();
        this->prev_time = this->cur_time;
    }

}

double Vin_Convert::get_stored_reading(){
    // return stored
    return reading;
}

double Vin_Convert::get_stored_scaled_reading(){
    // return stored
    return reading_scaled;
}

void Vin_Convert::set_scaling(double m, double b){
    // set scaling
    this->scaleM = m;
    this->scaleB = b;
}

double Vin_Convert::volt_read(){
  // reads analog input, returning 0-5
  int val = analogRead(IN150V); // value between 0-1023
  double voltage = (val/1023.0)*5.0; // convert to voltage

  return voltage;
}

double Vin_Convert::store_volt_scaled_read(){
    // reads 150V analog input, returning scaled value through global variables
    double voltage_5v = store_volt_read(); // read 0-5V
    double voltage_150v = voltage_5v * this->scaleM + this->scaleB; // scale to 0-150V
    this->reading_scaled = voltage_150v; // set global variable
    
    return voltage_150v;
}
