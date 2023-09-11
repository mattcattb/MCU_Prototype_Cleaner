#pragma once    
#include <Arduino.h>

namespace gen{ // general function helpers

void quick_digital_write(int pin, int value);

void quick_digital_write(int pin, int value){
// quickly does digital write on pin according to boolean value
    if(value == 1){
        digitalWrite(pin, HIGH); 
    }else if(value == 0){
        digitalWrite(pin, LOW);
    }
}

}
