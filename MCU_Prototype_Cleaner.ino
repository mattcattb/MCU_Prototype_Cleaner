#include "src/pins.h"
#include "src/equations.h"

#include "src/Segment/Segment_Display.h"
#include "src/Vin/Vin_Convert.h" 

#include "src/Motor/Motor_Driver.h"
#include "src/LED/LED_Driver.h"

#include <avr/sleep.h>
#include <avr/power.h>

void setup_timer1();
void voltage_calc_phase();

// voltage of truck
double Vt_truck;

double R_Line; // resistance of line 

// power 
double Pd_bias;
double Pd_LEDL;
double Pd_LEDR;

// to track cycle of timer
int cycle = 0;

// constant n_eff, Voltage out
const double n_eff = 0.7;
const double Vt_out = 24;

Motor_Driver motor_driver;
LED_Driver led_driver;

Segment_Display seg_disp;
Vin_Convert vin_convert;

void setup() {

  // scaling variables: 0v = 170Vin, 130V = 0Vin
  int vin_scaleM = -8; // slope of 150V scaling
  int vin_scaleB = 170; // y-intercept of 150V scaling

  // scaling for LED current (0-1.5 amps) 
  double led_scaleM = 1.5;
  double led_scaleB = 0;

  // begin serial communication 
  Serial.begin(9600);

  // set scaling for Vin voltage and LED current
  vin_convert.set_scaling(vin_scaleM, vin_scaleB);
  led_driver.set_scaling(led_scaleM, led_scaleB);

  // calculate voltage and resistance values
  voltage_calc_phase();

  // setup the Timer Interrupts
  setup_timer1();

  // Set sleep mode to power save, Timer1 is still active in this mode
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);

  // Enable sleep mode
  sleep_enable();

}

void loop() {
  sleep_mode(); 
}

ISR(TIMER1_COMPA_vect){
  // timer 1 at 240 hz

  // check Vt2 (every 150 ms) and recalc truck voltage vtruck
  if (cycle/240.0 >= 150.0){
    double Vt_2 = vin_convert.read();
  } 

  // if truck increases to 160V, check Pdbias val by turning off 1 LED (alternate), waiting 50ms measruing VT1 
  // turning LED back on and using equation 7 to recalculate Pdbias
  // turn motor to raise the lift if there is minimal change in pdbias

  // if Vtruck decreases from above 160V to below 155V stop raising the lift (turn motor off)

  // if Vtruck decreases to 150V check Pdbias val by turning off one LED, waiting 50ms measuring VT1 turning LED back on and 
  // then using equation 7 to recalculate the Pdbias
  // turn the motor on to lower lift if there is "minimal" change in pdbias

  // if truck voltage incrased from below 150V to 155V stop lowering the lift (turn motor off)
  


  // display scaled reading
  seg_disp.update_disp(100);
  cycle ++; 
}

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

void voltage_calc_phase(){
  // calculate vt0, vt1, vt2, R_line, Vt_truck, Pd_LEDL, Pd_LEDR

  // measure vt0 through averaging 100 samples
  double Vt_0 = vin_convert.read(100);

  // turn left LED light and wait 30 ms
  led_driver.write(1, 0);
  delay(30);

  // measure Vt1 and I_LED.left over 100 samples avg
  //? equation sheet says ILED.l = 1 A... how do I recalulate it...
  double Vt_1 = vin_convert.read(100);
  double I_LEDL = led_driver.read(100);

  // turn right LED on (both on) and wait 50 ms
  led_driver.write(1, 1);
  delay(50);

  // measure Vt2 and I_LED.right over 200 samples avg
  double Vt_2 = vin_convert.read(200);
  double I_LEDR = led_driver.read(200);

  // calculate PdLEDL, PdLEDR, PdBias
  Pd_LEDL = (I_LEDL*Vt_out)/n_eff;
  Pd_LEDR = (I_LEDR*Vt_out)/n_eff;
  
  // using equation 8 calculate RLine and store as constant 
  R_Line = R_LINE_EQ_Two(Vt_0, Vt_1, Vt_2, Pd_LEDL, Pd_LEDR);

  // using equation 7 calculate external bias power pdbias and store as semi-constant variable
  Pd_bias = Pd_bias_one_two(Vt_0, Vt_1, Pd_LEDL, R_Line);
  
  // using equation 3 measure truck voltage Vtruck
  Vt_truck = two_load_V_truck(Vt_2, Pd_bias, Pd_LEDL, Pd_LEDR, R_Line);

  //! keep rolling 100 sample over 100ms average of Vt2 (operating input voltage)

  unsigned long interval = 100;
  unsigned long start_millis = millis();
  unsigned long curMillis = millis();
  
  while(curMillis < start_millis + interval){
    
    Vt_2 = 
    curMillis = millis();
  } 

}
