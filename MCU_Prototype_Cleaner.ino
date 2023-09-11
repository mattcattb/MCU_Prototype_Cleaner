#include "generalHelpers.h"
#include "driversHelper.h"
#include "Vin150Helper.h"
#include "segDisplayHelper.h"

void setup() {

  int scaleM = 30; // slope of 150V scaling
  int scaleB = 0; // y-intercept of 150V scaling

  // begin serial communication 
  Serial.begin(9600);

  // setup pins
  VinConvert::pin_setup();

  led::pin_setup();
  motor::pin_setup();

  segDisp::pin_setup();

  // set scaling and get introductory reading from 150V input
  VinConvert::set_scale(scaleM, scaleB); // set scale for 150V input
  VinConvert::get_reading(); // get reading from 150V input

}

void loop() {

  // possibly update following parts of device if enough time has passed!

  // update display
  segDisp::Display();
  
  // update reading from 150V input
  VinConvert::update_reading();
  
  // update drivers
  motor::update();
  led::update();

}

