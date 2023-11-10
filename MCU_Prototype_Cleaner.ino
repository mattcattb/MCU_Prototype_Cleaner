#include "pins.h"

#include "Segment_Display.h"
#include "Vin_Convert.h"

#include "Motor_Driver.h"
#include "LED_Driver.h"

#include <avr/sleep.h>
#include <avr/power.h>

Motor_Driver motor_driver;
LED_Driver led_driver;

Segment_Display seg_disp;
Vin_Convert vin_convert;

double Vt_truck;

double Vt_0;
double Vt_1; 
double Vt_2;

double I_LEDL;
double I_LEDR;

double R_Line; // resistance of line 

double Pd_bias;
double Pd_LEDL;
double Pd_LEDR;

void setup_timer1(){

  cli(); // stop interupts

  TCCR1A = 0; // set TCCR1A reg to 0
  TCCR1B = 0; // set TCCR1B reg to 0
  TCNT1 = 0; // set counter to 0

  OCR1A = 999; // Compare Match Register Value
  TCCR1A |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10); // 64 bit prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interupt 

  sei(); // enable interupts again 

}

void setup() {

  // scaling variables: 0v = 170Vin, 130V = 0Vin
  int scaleM = -8; // slope of 150V scaling
  int scaleB = 170; // y-intercept of 150V scaling

  // begin serial communication 
  Serial.begin(9600);

  // set scaling
  vin_convert.set_scaling(scaleM, scaleB);

  // setup the Timer Interrupts
  setup_timer1()

  // Set sleep mode to power save, Timer1 is still active in this mode
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);

  // Enable sleep mode
  sleep_enable();

  // measure vt0 through averaging 100 samples

  // turn left LED light and wait 30 ms
  led_driver.set_channels(1, 0);
  delay(30);

  // measure Vt1 and I_LED.left over 100 samples avg
  //? equation sheet says ILED.l = 1 A... how do I recalulate it...
  // Vt1 = ... idk
 
  // turn right LED on (both on) and wait 50 ms

  led_driver.set_channels(1, 1);
  delay(50);

  // measure Vt2 and I_LED.right over 200 samples avg
  //? what is Vt2? equation sheet says I_LED.right is 1.1 A 

  // using equation 8 calculate RLine and store as constant 
  // RLine = (Vt0 * VT1)/(PdLED.I * Vt0/(Vt0-Vt1) + Pdbias)

  // using equation 7 calculate external bias power pdbias and store as semi-constant variable
  // pdbias = ((Vt0 - Vt1)/Rline - PdLED.l/Vt1)*(Vt0 * Vt1)/(Vt0 - Vt1) 

  // using equation 3 measure truck voltage Vtruck
  // Vt_truck = 

  // keep rolling 100 sample over 100ms average of Vt2 (operating input voltage)

}

void loop() {
  sleep_mode(); 
}

ISR(TIMER1_COMPA_vect){
  // timer 1 at 240 hz

  // check Vt2 (every 150 ms) and recalc truck voltage vtruck

  // if truck increases to 160V, check Pdbias val by turning off 1 LED (alternate), waiting 50ms measruing VT1 
  // turning LED back on and using equation 7 to recalculate Pdbias
  // turn motor to raise the lift if there is minimal change in pdbias

  // if Vtruck decreases from above 160V to below 155V stop raising the lift (turn motor off)

  // if Vtruck decreases to 150V check Pdbias val by turning off one LED, waiting 50ms measuring VT1 turning LED back on and 
  // then using equation 7 to recalculate the Pdbias
  // turn the motor on to lower lift if there is "minimal" change in pdbias

  // if truck voltage incrased from below 150V to 155V stop lowering the lift (turn motor off)
  

  // update all components
  motor_driver.update();
  led_driver.update(); 
  vin_convert.update_reading();

  // get already stored scaled reading
  double vin_reading_scaled = vin_convert.latest_reading();

  // display this scaled reading if possible
  seg_disp.update_disp(vin_reading_scaled);
}