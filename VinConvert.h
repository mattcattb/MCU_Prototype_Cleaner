#pragma once
#include <Arduino.h>

class VinConvert{

    const uint8_t in_pin;

    int scaleM;
    int scaleB;

    double reading_scaled;

    unsigned long prev_time; 
    unsigned long cur_time;
    unsigned long reading_delay;

public:

    void update_reading();
    double get_reading();
    double volt_read();
    double volt_scaled_read();

    VinConvert(double m, double b);

}