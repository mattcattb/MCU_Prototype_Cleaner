#include "VinConvert.h"
#include "pins.h"

void VinConvert::update_reading(){
    
    this->cur_time = millis();

    // possibly update reading from 150V input
    if(this->cur_time - this->prev_time > this->reading_delay){
        volt_scaled_read();
        this->prev_time = this->cur_time;
    }

}

double VinConvert::get_reading(){
    // get reading from 150V input
    return this->reading_scaled;
}

VinConvert::VinConvert(double scaleM, double scaleB){
    
    // set scaling
    this->scaleM = scaleM;
    this->scaleB = scaleB;
    
    // constructor
    this->prev_time = millis();
    this->reading_delay = 20;

    // set input pins
    pinMode(IN150V, INPUT);

}

double VinConvert::volt_read(){
  // reads 150V analog input, returning 0-5
  int val = analogRead(IN150V); // value between 0-1023
  double voltage = (val/1023.0)*5.0; // convert to voltage

  return voltage;
}

double VinConvert::volt_scaled_read(){
    // reads 150V analog input, returning scaled value through global variables
    double voltage_5v = volt_read(); // read 0-5V
    double voltage_150v = voltage_5v * this->scaleM + this->scaleB; // scale to 0-150V
    this->reading_scaled = voltage_150v; // set global variable
    
    return voltage_150v;
}
