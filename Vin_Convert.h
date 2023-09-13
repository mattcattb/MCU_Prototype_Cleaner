#pragma once
#include <Arduino.h>
#include "pins.h"

//! NEED TO FIX FUCKED UP THINGS READING!!

class Vin_Convert{

    const uint8_t in_pin;

    // scaling factors
    int scaleM = 0;
    int scaleB = 0;

    // actual reading and scaled reading

    int analog_data; // 0-1023

    double volt_data; // 0-5 volt read
    double volt_scaled_data; // scaled reading according to scalM and scaleB

    unsigned long prev_time; 
    unsigned long cur_time;
    unsigned long reading_delay = 20;

    int analog_read(); // digital read from i/o pin

public:

    Vin_Convert();

    // set scaling factor
    void set_scaling(double m, double b);

    // update reading if ms has passed
    void update_reading();

    // have vin take reading   
    void volt_read(); // take and store reading in digital_reading, volt_reading, volt_reading_scaled 

    // get already stored readings
    int get_stored_digital_reading(); // 0- 1023
    double get_stored_reading(); // 0-5
    double get_stored_scaled_reading(); // between scalings

};

Vin_Convert::Vin_Convert(){

    // set timing
    this->prev_time = millis();

    // set input pins
    pinMode(IN150V, INPUT);

    // take 1 reading to initialize
    volt_read();

}

void Vin_Convert::set_scaling(double m, double b){
    // set scaling
    this->scaleM = m;
    this->scaleB = b;
}

void Vin_Convert::update_reading(){
    // update reading if enough time has passed

    this->cur_time = millis();

    // if enough time has passed, update reading
    if(this->cur_time - this->prev_time > this->reading_delay){
        volt_read();
        this->prev_time = this->cur_time;
    }

}

int Vin_Convert::get_stored_digital_reading(){
    // return stored digital reading
    return this->analog_data;
}

double Vin_Convert::get_stored_reading(){
    // return stored volt reading
    return this->volt_data;
}

double Vin_Convert::get_stored_scaled_reading(){
    // return stored scaled reading
    return this->volt_scaled_data;
}

int Vin_Convert::analog_read(){
  // reads analog pin, returning val between 0-1023
  int val = analogRead(IN150V); // 

  return val;
}

void Vin_Convert::volt_read(){
    // reads 150V analog input, returning scaled value through global variables
    int temp_analog_read = analog_read(); // read int 0-1023
    double voltage_5v = temp_analog_read * 5.0/1023.0; // scale to 0-5V
    double voltage_150v = voltage_5v * this->scaleM + this->scaleB; // scale to 0-150V
    
    // set each class variable
    this->analog_data = voltage_5v; // store digital reading
    this->volt_data = voltage_5v; // store 5v reading in class variable
    this->volt_scaled_data = voltage_150v; // store scaled reading in calss variable
    
}
