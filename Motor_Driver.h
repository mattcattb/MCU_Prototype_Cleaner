#pragma once
#include <Arduino.h>
#include "pins.h"

class Motor_Driver{

    unsigned long reading_delay = 20; // how long to wait between readings
    unsigned long prev_time = 0;
    unsigned long cur_time;

    int L_Sense;
    int R_Sense;

    // states: sleep = 0; brake = 1; left = 2; right = 3
    int state;

    void pin_setup();
    void Motor_Driver_Control(int in_L, int in_R, int INH); // motor control

    // write stored state to motor driver
    void write_state();

public:

    Motor_Driver(); // constructor for motor driver
    void update(); // update based on delay

    void set_state(int state); // set state

    
    // get L_sense or R_Sense
    int get_L_Sense();
    int get_R_Sense();

};

Motor_Driver::Motor_Driver(){
    pin_setup();
    this->prev_time = millis();
    this->state = 0; // current state is sleep
    write_state(); // write motor to sleep
}

void Motor_Driver::pin_setup(){
    // set pins i/o
    pinMode(M_D_R, OUTPUT);
    pinMode(M_D_L, OUTPUT);
    pinMode(M_EN, OUTPUT);
    pinMode(SenseMR_pin, INPUT);
    pinMode(SenseML_pin, INPUT);

}

void Motor_Driver::update(){
    // update motor if enough time has passed

    this->cur_time = millis();

    if (this->cur_time - this->prev_time > this->reading_delay){
        // write to motor currently set state
        this->prev_time = this->cur_time;
        write_state();

    }
}

void Motor_Driver::write_state(){
    // write to motor driver based on state

    switch (this->state)
    {
    case 0: // sleep
        Motor_Driver_Control(0, 0, 0);
        break;
    
    case 1: // brake
        Motor_Driver_Control(1, 0, 0);
        break;

    case 2: // left
        Motor_Driver_Control(1, 1, 0);
        break;

    case 3: // right
        Motor_Driver_Control(1, 0, 1);
        break;

    }
}

void Motor_Driver::set_state(int state){
    // set state of motor driver
    this->state = state;
}

void Motor_Driver::Motor_Driver_Control(int in_L, int in_R, int INH){

    /*
    * in_L: input for left motor (High or Low)
    * int_R: input for right motor
    * INH: inhibit, set low to sleep, high to allow motor use (M_EN)
    * L_Sense: Current Sense Left Driver (0-1023)
    * R_Sense: Current Sense Right Driver (0-1023)
    */

    // set left motor
    quick_digital_write(M_D_L, in_L);

    // set right motor
    quick_digital_write(M_D_R, in_R);

    // set inhibit
    quick_digital_write(M_EN, INH);

    // write sense variables
    this->L_Sense = analogRead(SenseML_pin);
    this->R_Sense = analogRead(SenseMR_pin); 

}

int Motor_Driver::get_L_Sense(){
  return this->L_Sense;
}
int Motor_Driver::get_R_Sense(){
  return this->R_Sense;  
}
