#pragma once
#include <Arduino.h>
#include "../pins.h"

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
