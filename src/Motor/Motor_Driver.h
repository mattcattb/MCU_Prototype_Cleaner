#pragma once
#include <Arduino.h>
#include "../pins.h"

#define OFF 0
#define LEFT 1
#define RIGHT 2


class Motor_Driver{

    int L_Sense;
    int R_Sense;

    // states: sleep = 0; brake = 1; left = 2; right = 3
    int state;

    void pin_setup();
    void Motor_Driver_Control(int in_L, int in_R, int INH); // motor control


public:

    Motor_Driver(); // constructor for motor driver

    // set state of motor (0: sleep, 1: brake, 2: left, 3: right)
    void set_state(int state); 

    // write stored state to motor driver
    void write_state();
    
    // get L_sense or R_Sense
    int get_L_Sense();
    int get_R_Sense();

    void quick_digital_write(const int pin, int value){
    // quickly does digital write on pin according to boolean value
        if(value == 1){
            digitalWrite(pin, HIGH); 
        }else if(value == 0){
            digitalWrite(pin, LOW);
        }
    }

};
