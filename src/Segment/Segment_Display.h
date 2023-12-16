#pragma once
#include <Arduino.h>
#include "../pins.h"

class Segment_Display{

  const uint8_t ON = LOW;
  const uint8_t OFF = HIGH;

  // array of segment pin addresses
  uint8_t segment_pins[7] = {D_A_pin, D_B_pin, D_C_pin, D_D_pin, D_E_pin, D_F_pin, D_G_pin}; 

  // encodings for each number, for segments 0bABCDEFG being on or off (here, 0 is on, 1 is off)
  //                                  0           1         2          3          4           5         6         7           8           9
  uint8_t segment_encodings[10] = {0b0000001, 0b1001111, 0b0010010, 0b0000110, 0b1001100, 0b0100100, 0b0100000, 0b0001111, 0b0000000, 0b0001100};

  unsigned long seg_delay = 2; // ms delay between segment displays
  unsigned long prev_time; 
  unsigned long cur_time;

  int state = 0; // current state of 3 for each segment
  int num_states = 3; // number of states for each segment

  double display_value; // value to be showing

  void set_display_digit(int value, int idx);
  void pin_setup();

  int get_digit(double display_val, int digit_idx); // get specific digit of float

public:

  Segment_Display();

  // cycle state showing digit
  void update_state(); 

  // change the value being shown 
  void set_display_value(double val); // run a state change for what display is on

  void quick_digital_write(const int pin, int value){
  // quickly does digital write on pin according to boolean value
      if(value == 1){
          digitalWrite(pin, HIGH); 
      }else if(value == 0){
          digitalWrite(pin, LOW);
      }
  }

};

