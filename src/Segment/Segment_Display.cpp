#include "Segment_Display.h"

Segment_Display::Segment_Display(){
  // set pins
  pin_setup();

  this->prev_time = millis();

}

void Segment_Display::set_display(int value, int idx){
  /*

   set display idx to value 

   value: what number to set 7 seg to 
   display_idx: which segment to turn on. 0, 1, 2 
   enable_dot: turn on dot at end of segment display
  */
  
  // set all low
  analogWrite(SEG1, LOW);
  analogWrite(SEG2, LOW);
  analogWrite(SEG3, LOW);

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

int Segment_Display::get_digit(double value, int digit_idx){
  // get the digit at the digit_idx of the display_val
  for(int i = 0; i < digit_idx; i += 1){
    value = value/10.0;  
  }

  // set that display to the number 
  int digit = ((int)value)%10;

  return digit;
}


void Segment_Display::update_disp(double display_val){
  // see if enough time has passed, if so, turn on segment to the reading and go to next state
  this->cur_time = millis();

  if(this->cur_time - this->prev_time > this->seg_delay){
    // turn on specific digit to the reading

    // prev time is now cur_time
    this->prev_time = this->cur_time;

    // get the digit to display 
    int digit_idx = this->num_states - this->state;
    int digit = get_digit(display_val, digit_idx);

    // set that display to the number 
    set_display(state, digit);

    // increase state
    state = (state + 1)%(num_states);
  }

}

void Segment_Display::pin_setup(){

  // set seg switcher to output
  pinMode(SEG1, OUTPUT);
  pinMode(SEG2, OUTPUT);
  pinMode(SEG3, OUTPUT);

  // set digit pins to output
  pinMode(D_A, OUTPUT);
  pinMode(D_B, OUTPUT);
  pinMode(D_C, OUTPUT);
  pinMode(D_D, OUTPUT);
  pinMode(D_E, OUTPUT);
  pinMode(D_F, OUTPUT);
  pinMode(D_G, OUTPUT);
  pinMode(D_dp, OUTPUT);
}