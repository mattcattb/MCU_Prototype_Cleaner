#include "pins.h"

#include "Segment_Display.h"
#include "Vin_Convert.h"

#include "Motor_Driver.h"
#include "LED_Driver.h"

Motor_Driver motor_driver;
LED_Driver led_driver;

Segment_Display seg_disp;
Vin_Convert vin_convert;


void setup() {

  // scaling variables
  int scaleM = 30; // slope of 150V scaling
  int scaleB = 0; // y-intercept of 150V scaling

  // begin serial communication 
  Serial.begin(9600);

  // set scaling
  vin_convert.set_scaling(scaleM, scaleB);

}

void loop() {

  // update all components
  motor_driver.update();
  led_driver.update(); 
  vin_convert.update_reading();

  // get already stored scaled reading
  double vin_reading_scaled = vin_convert.get_stored_scaled_reading();

  // display this scaled reading if possible
  seg_disp.update_disp(vin_reading_scaled);

}
