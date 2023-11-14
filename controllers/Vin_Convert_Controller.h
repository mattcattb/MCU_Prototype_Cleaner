#pragma once
#include <Arduino.h>
#include <cppQueue.h>
#include "pins.h"

// more complex Vin_Convert

class Vin_Convert_Controller{

    // scaling factors
    int scaleM = 0;
    int scaleB = 0;

    // latest reading
    double latest_reading; // scaled reading according to scalM and scaleB

    unsigned long reading_delay = 20; // take reading every 20 ms
    unsigned long prev_time; 
    unsigned long cur_time;

    int window_size = 10; // number of averages to take for reading
    double rolling_average = 0; // average of volt_scaled_data

    // (size_t size_rec, uint16_t nb_recs=20, QueueType type=FIFO, bool overwrite=false, void * pQDat=NULL, size_t lenQDat=0

    cppQueue *volt_scaled_queue; // pointer to queue of readings

    int analog_read(); // digital read from i/o pin

    // have vin take reading and store to class variables   
    void volt_read();  
    void calc_rolling_avg(); // calculate rolling avg based on volt_scaled_data

public:

    double analog_read_scaled(); // take a scaled voltage reading and return it
    double analog_read_scaled(int avg_times); // take an average over that many readings

    Vin_Convert_Controller();
    Vin_Convert_Controller(int window_size, int delay_time);

    // set scaling factor
    void set_scaling(double m, double b);

    // check if enough time has passed to update reading
    void update_reading();

    // average of last 10 readings
    double get_reading(); // rolling average of last 10 readings

};

Vin_Convert_Controller::latest_reading(){
    // return rolling average reading
    return this->rolling_average;
}

Vin_Convert_Controller::Vin_Convert_Controller(){

    // set input pins
    pinMode(IN150V, INPUT);

    // create queue
    this->volt_scaled_queue = new cppQueue(sizeof(double), this->window_size, FIFO, true);

    // take 1 reading to initialize
    volt_read();

    // set timing
    this->prev_time = millis();

}


Vin_Convert_Controller::Vin_Convert_Controller(int window_size, int delay_time){

    // set input pins
    pinMode(IN150V, INPUT);

    this->window_size = window_size;
    this->reading_delay = delay_time;

    // create queue
    this->volt_scaled_queue = new cppQueue(sizeof(double), window_size, FIFO, true);

    // take 1 reading to initialize
    volt_read();

    // set timing
    this->prev_time = millis();

}

void Vin_Convert_Controller::set_scaling(double m, double b){
    // set scaling
    this->scaleM = m;
    this->scaleB = b;
}

void Vin_Convert_Controller::update_reading(){
    // update reading if enough time has passed

    this->cur_time = millis();

    // if enough time has passed, update reading
    if(this->cur_time - this->prev_time > this->reading_delay){
        volt_read();
        this->prev_time = this->cur_time;
    }

}

void Vin_Convert_Controller::calc_rolling_avg(){
    // use latest reading to update rolling average and queue

    int queue_size = this->volt_scaled_queue->size();

    if(queue_size > this->window_size){
        // if queue is too big, remove oldest reading and adjust average accordingly
        double oldest_reading = volt_scaled_queue->front();
        this->volt_scaled_queue->pop();
        this->rolling_average -= ((oldest_reading) * (1/queue_size)); // remove from rolling average
    }

    this->volt_scaled_queue->push(this->latest_reading); // add latest reading to queue
    this->rolling_average += (this->latest_reading) * (1/this->window_size); // add to rolling average


}

int Vin_Convert_Controller::analog_read(){
  // reads analog pin, returning val between 0-1023
  int val = analogRead(IN150V); 
  return val;
}

double Vin_Convert_Controller::analog_read_scaled(){
    // reads 150V analog input, returning scaled value through global variables
    int temp_analog_read = analog_read(); // read int 0-1023
    double voltage_5v = temp_analog_read * 5.0/1023.0; // scale to 0-5V
    double voltage_150v = voltage_5v * this->scaleM + this->scaleB; // scale to 0-150V

    return voltage_150v;
}

double Vin_Convert_Controller::analog_read_scaled(int avg_times){ 
    // take an average over that many readings
    double sum = 0;

    for(int i = 0; i < avg_times; i += 1){
        sum += analog_read_scaled();
    }

    double average = sum\avg_times;
    return average;
}

void Vin_Convert_Controller::volt_read(){
    // reads 150V analog input, returning scaled value through global variables
    int temp_analog_read = analog_read(); // read int 0-1023
    double voltage_5v = temp_analog_read * 5.0/1023.0; // scale to 0-5V
    double voltage_150v = voltage_5v * this->scaleM + this->scaleB; // scale to 0-150V
    
    // set each class variable
    this->latest_reading = voltage_150v; // store scaled reading 
    
    // calculate rolling average with this reading
    calc_rolling_avg();
}