#pragma once
#include <Arduino.h>

class MotorDriver{

    unsigned long reading_delay = 20; // how long to wait between readings
    unsigned long prev_time = 0;
    unsigned long cur_time;

    int L_Sense;
    int R_Sense;

public:

    MotorDriver();

    void Motor_Driver_Control(int in_L, int in_R, int INH, int *L_Sense, int *R_Sense){
    void update();

}