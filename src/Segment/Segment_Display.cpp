#include "Segment_Display.h"

// ======= public Functions =======

Segment_Display::Segment_Display(){
  // set pins
  pin_setup();

  // first value to be displayed is 0
  this->display_value = 0; 

  this->prev_time = millis();

}


void Segment_Display::update_state(){
  // if enough time has passed, cycle state and turn on that digit
  
  this->cur_time = millis();

  if(this->cur_time - this->prev_time > this->seg_delay){
    // turn on specific digit to the reading

    // prev time is now cur_time
    this->prev_time = this->cur_time;

    // get specific digit from display_value to be displayed
    int digit_idx = this->num_states - this->state;
    int digit = get_digit(this->display_value, digit_idx);

    // set that display to the number 
    set_display_digit(state, digit);

    // increase state
    state = (state + 1)%(num_states);
  }

}

void Segment_Display::set_display_value(double val){
  // sets what value to be displayed
  // TODO incorporate errors to be displayed!
  this->display_value = val;

}

// ======= private Functions =======

void Segment_Display::set_display_digit(int value, int idx){
  /*
   set display digit of idx to value 

   value: what number to set 7 seg to (0 to 9)
   display_idx: which segment to turn on. 0, 1, 2 
   enable_dot: turn on dot at end of segment display
  */
  
  // set all off (off is low macro)
  analogWrite(SEG1_pin, OFF);
  analogWrite(SEG2_pin, OFF);
  analogWrite(SEG3_pin, OFF);

  uint8_t val_encoding = segment_encodings[value];
  
  for (int i = 0; i < 7; i += 1){

    // get the bit at the end of the encoding
    int on = 0b1 & val_encoding;

    // find analog pin of current segment
    int cur_segment = segment_pins[7- i - 1];
    
    // set that segment pin to on or off
    quick_digital_write(cur_segment, on);

    val_encoding = val_encoding/2; // shift right
  }

  // now send the signals to the proper segment
  switch(idx){
    case 0:
      analogWrite(SEG1_pin, ON);
      break;
    case 1:
      analogWrite(SEG2_pin, ON);
      break;
    case 2:
      analogWrite(SEG3_pin, ON);
      break;
  }
  
}

int Segment_Display::get_digit(double value, int digit_idx){
  // helper to get the digit at the digit_idx of the display_val
  
  for(int i = 0; i < digit_idx; i += 1){
    value = value/10.0;  
  }

  // set that display to the number 
  int digit = ((int)value)%10;

  return digit;
}

void Segment_Display::pin_setup(){
  // initialize all pins during setup

  // set seg switcher to output
  pinMode(SEG1_pin, OUTPUT);
  pinMode(SEG2_pin, OUTPUT);
  pinMode(SEG3_pin, OUTPUT);

  // set digit pins to output
  pinMode(D_A_pin, OUTPUT);
  pinMode(D_B_pin, OUTPUT);
  pinMode(D_C_pin, OUTPUT);
  pinMode(D_D_pin, OUTPUT);
  pinMode(D_E_pin, OUTPUT);
  pinMode(D_F_pin, OUTPUT);
  pinMode(D_G_pin, OUTPUT);
  pinMode(D_dp_pin, OUTPUT);
}

