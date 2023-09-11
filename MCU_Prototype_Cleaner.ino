#include "generalHelpers.h"
#include "driversHelper.h"
#include "Vin150Helper.h"
#include "segDisplayHelper.h"

// unspecified pins 
const uint8_t AREF = 20;
const uint8_t AVCC = 18;

void setup() {

  // begin serial communication 
  Serial.begin(9600);

  // setup pins
  VinConvert::pin_setup();

  led::pin_setup();
  motor::pin_setup();

  segDisp::pin_setup();

  VinConvert::set_scale(30.0, 0.0); // set scale for 150V input
}

void loop() {

  // possibly update following parts of device if enough time has passed!
  segDisp::Display();
  VinConvert::update_reading();
  motor::update_motor();
  led::update_led();

}

