#include "Vin_Convert.h"

Vin_Convert::Vin_Convert(double m, double b){
    pinMode(IN150V_pin, INPUT);
    scaleM = m;
    scaleB = b;
}

void Vin_Convert::set_scaling(double m, double b){
    scaleM = m;
    scaleB = b;
}

int Vin_Convert::analog_read(){ 
    // 0-1023 basic analog read 
    adc_reading = analogRead(IN150V_pin);
    
    return adc_reading;
}

double Vin_Convert::read(int n){ 
    // reading average over n readings
    double sum = 0;
    double volt; 
    for(int i = 0; i < n; i += 1){
        volt = ((double)analog_read())*5.0/1023.0;
        sum += (volt * scaleM + scaleB);
    }

    return sum/n;
}
