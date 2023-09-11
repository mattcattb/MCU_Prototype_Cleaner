#pragma once    
#include <Arduino.h>

#include "Vin150Helper.h"

namespace segDisp{

void set_display(int value, int display_idx, int enable_dot = 0);
void Display();
void pin_setup();

// Segment Displays
const uint8_t SEG1 = 12; 
const uint8_t SEG2 = 13;
const uint8_t SEG3 = 14;

// to segment display 
const uint8_t D_A = 30;
const uint8_t D_B = 31;
const uint8_t D_C = 32;
const uint8_t D_D = 1;
const uint8_t D_E = 2;
const uint8_t D_F = 9;
const uint8_t D_G = 10;
const uint8_t D_dp = 11;

uint8_t segment_pins[] = {D_A, D_B, D_C, D_D, D_E, D_F, D_G}; // array of segment pin addresses
uint8_t segment_encodings[] ={0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1110011}; // encodings for each number

unsigned long seg_delay = 2; // delay between segment displays
unsigned long prev_time = 0; 

int state = 0; // current state of 3 for each segment
int num_states = 3; // number of states for each segment

void set_display(int value, int display_idx, int enable_dot = 0){
  /*
   * value: what number to set 7 seg to 
   * display_idx: which segment to turn on. 0, 1, 2 
   * enable_dot: turn on dot at end of segment display
   */
  
  // set all low
  analogWrite(SEG1, LOW);
  analogWrite(SEG2, LOW);
  analogWrite(SEG3, LOW);

  uint8_t val_encoding = segment_encodings[value];
  
  for (int i = 0; i < 10; i += 1){

    // get the bit at the end of the encoding
    int on = 0b1 & val_encoding;

    // find analog pin of current segment
    int cur_segment = segment_pins[7-i - 1];
    
    // set that segment pin to on or off
    gen::quick_digital_write(cur_segment, on);

    val_encoding = val_encoding/2; // shift right
  }

  // turn dot on or off
  gen::quick_digital_write(D_dp, enable_dot);

  // now send the signals to the proper segment
  switch(display_idx){
    case 0:
      analogWrite(SEG1, HIGH);
      break;
    case 1:
      analogWrite(SEG2, HIGH);
      break;
    case 2:
      analogWrite(SEG3, HIGH);
      break;
  }
  
}

void Display(){
    // see if enough time has passed, if so, turn on segment to the reading and incriment
    cur_time = millis();

    if(cur_time - prev_time > seg_delay){
        
        // turn on one segment at a time now
        prev_time = cur_time;

        // turn on that segment with the val from disp_digits
        double reading = VinConvert::get_reading();

        // get the digit to display 
        for(int i = 0; i <= state; i += 1){
        reading = reading/10.0;  
        }

        int digit = (int)reading%10;

        segDisp::set_display(state, digit);

        // increase state
        state = (state + 1)%(num_states);
  }

}

void pin_setup(){
    pinMode(SEG1, OUTPUT);
    pinMode(SEG2, OUTPUT);
    pinMode(SEG3, OUTPUT);

    pinMode(D_A, OUTPUT);
    pinMode(D_B, OUTPUT);
    pinMode(D_C, OUTPUT);
    pinMode(D_D, OUTPUT);
    pinMode(D_E, OUTPUT);
    pinMode(D_F, OUTPUT);
    pinMode(D_G, OUTPUT);
    pinMode(D_dp, OUTPUT);
}

}