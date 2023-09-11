#include "MotorDriver.h"
#include "generalHelpers.h"
#include "pins.h"

MotorDriver::MotorDriver(){
    
    // set pins i/o
    pinMode(M_D_R, OUTPUT);
    pinMode(M_D_L, OUTPUT);
    pinMode(M_EN, OUTPUT);
    pinMode(SenseMR, INPUT);
    pinMode(SenseML, INPUT);

}

void MotorDriver::update(){
    // update motor if enough time has passed

    this->cur_time = millis();

    if (this->cur_time - this->prev_time > this->reading_delay){
        //! update motor!
        this->prev_time = this->cur_time;
    }

}

void MotorDriver::Motor_Driver_Control(int in_L, int in_R, int INH){

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
