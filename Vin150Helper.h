#pragma once
#include <Arduino.h>

namespace VinConvert{

void update_reading(); // update reading_scaled if enough time has passed
double get_reading(); // get reading from reading_scaled double
void set_scale(double m, double b); // set scaling factor for VinConvert
double volt_read(); // 5V read from IN150V pin val 
double volt_scaled_read(); // Reads from pin and returns scaled voltate

// pin def 
const uint8_t IN150V = 23;

// scaling variables
int scaleM; // slope of 150V scaling
int scaleB; // y-intercept of 150V scaling

double reading_scaled = 0; // reading from 150V input

// timing variables 
unsigned long reading_delay = 20; 
unsigned long prev_time = 0;
unsigned long cur_time; 

void update_reading(){
    
    cur_time = millis();

    // possibly update reading from 150V input
    if(cur_time - prev_time > reading_delay){
        volt_scaled_read();
        prev_time = cur_time;
    }

}

double get_reading(){
    // get reading from 150V input
    return reading_scaled;
}

void set_scale(double m, double b){
  // sets the scale for the 150V input
  scaleM = m;
  scaleB = b;
}

double volt_read(){
  // reads 150V analog input, returning 0-5
  int val = analogRead(IN150V); // value between 0-1023
  double voltage = (val/1023.0)*5.0; // convert to voltage

  return voltage;
}

double volt_scaled_read(){
  // reads 150V analog input, returning scaled value through global variables
  double voltage_5v = volt_read(); // read 0-5V
  double voltage_150v = voltage_5v * scaleM + scaleB; // scale to 0-150V
  reading_scaled = voltage_150v; // set global variable
  
  return voltage_150v;
}

void pin_setup(){
    pinMode(IN150V, INPUT);
}

}
