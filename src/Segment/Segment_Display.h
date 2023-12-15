#pragma once
#include <Arduino.h>
#include "../pins.h"

class Segment_Display{

  uint8_t segment_pins[7] = {D_A, D_B, D_C, D_D, D_E, D_F, D_G}; // array of segment pin addresses
  uint8_t segment_encodings[10] ={0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1110011}; // encodings for each number

  unsigned long seg_delay = 2; // ms delay between segment displays
  unsigned long prev_time; 
  unsigned long cur_time;

  int state = 0; // current state of 3 for each segment
  int num_states = 3; // number of states for each segment

  void set_display(int value, int idx);
  void pin_setup();

  int get_digit(double display_val, int digit_idx); // get specific digit of float

public:

  Segment_Display();
  void update_disp(double display_val); // update if timing correct

};
