#pragma once
#include <Arduino.h>
#include "pins.h"

class Motor_Driver{

    unsigned long reading_delay = 20; // how long to wait between readings
    unsigned long prev_time = 0;
    unsigned long cur_time;

    int L_Sense;
    int R_Sense;

public:

    Motor_Driver(); // constructor for motor driver

    void Motor_Driver_Control(int in_L, int in_R, int INH, int *L_Sense, int *R_Sense); // motor control
    void update(); // update based on delay

};

Motor_Driver::Motor_Driver(){
    
    // set pins i/o
    pinMode(M_D_R, OUTPUT);
    pinMode(M_D_L, OUTPUT);
    pinMode(M_EN, OUTPUT);
    pinMode(SenseMR, INPUT);
    pinMode(SenseML, INPUT);

}

void Motor_Driver::update(){
    // update motor if enough time has passed

    this->cur_time = millis();

    if (this->cur_time - this->prev_time > this->reading_delay){
        //! update motor!
        this->prev_time = this->cur_time;
    }

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
    gen::quick_digital_write(M_D_L, in_L);

    // set right motor
    gen::quick_digital_write(M_D_R, in_R);

    // set inhibit
    gen::quick_digital_write(M_EN, INH);

    // write sense variables
    this->L_Sense = analogRead(SenseML);
    this->R_Sense = analogRead(SenseMR); 

}
