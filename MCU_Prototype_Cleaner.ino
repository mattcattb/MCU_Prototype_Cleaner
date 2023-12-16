#include "src/pins.h"
#include "src/equations.h"

#include "src/Segment/Segment_Display.h"
#include "src/Vin/Vin_Convert.h" 

#include "src/Motor/Motor_Driver.h"
#include "src/LED/LED_Driver.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <cppQueue.h>

void setup_timer1();
void voltage_calc_phase();

// voltage of truck
double Vt_truck;

// resistance of line 
double R_Line; 

// power values
double Pd_bias;
double Pd_LEDL;
double Pd_LEDR;

// rolling average queue of voltages taken from vin!
int queue_max_size = 10; 
double vt_truck_avg = 0; // rolling average value of vt_truck
cppQueue vt_truck_rolling_queue(sizeof(double), queue_max_size, LIFO);

// constant n_eff, Voltage out
const double n_eff = 0.7;
const double Vt_out = 24;

Motor_Driver motor_driver;
LED_Driver led_driver;

Segment_Display seg_disp;
Vin_Convert vin_convert;

void setup() {

  // take a 10 second delay first
  delay(10000);

  set_scalings();

  // begin serial communication 
  Serial.begin(9600);

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
  // this loop executes 240 times a second, or every 0.0041 seconds (4 ms)

  take_reading();
  
  display_voltage();

  change_segment_state(); 

  update_motor_state();

  motor_driver.update();

<<<<<<< HEAD
  // get already stored scaled reading
  double vin_avg_scaled = vin_convert.get_latest_reading();

  // display this scaled reading if possible
  seg_disp.update_disp(vin_reading_scaled);
=======
>>>>>>> Src-Organized-Filestructure
}


// ===== Loop Functions =====

// TODO figure out how to determine if action should be updated or not!

void take_reading(){
  // if enough time has passed, take in reading!

  bool ready_to_update = true;

  if (ready_to_update){
    // TCNT1 is a cycle value 
    
    // TODO properly calculate vt_2 using pdbias, pdL and pdR, and RLine
    double vt_2 = vin_convert.read();
    double vt_truck = two_load_V_truck(vt_2, Pd_bias, Pd_LEDL, Pd_LEDR, R_Line);

    // add this to the rolling average queue!
    update_rolling_avg(vt_truck);
  } 
}

// macros for motor states
#define OFF 0
#define LEFT 1
#define RIGHT 2

void update_motor_state(){

  // change motor if enough time using vt_truck_avg (every 150ms?)

  // 0: sleep, 1: brake, 2: left, 3: right
  bool ready_to_update = true;

  if (ready_to_update){

    if (vt_truck_avg >= 160){
      // if truck increases to 160V, check Pdbias val by turning off 1 LED (alternate), waiting 50ms measuring VT1 
      // turning LED back on and using equation 7 to recalculate Pdbias
      // turn motor to raise the lift (go right)
      
      motor_driver.set_state(RIGHT);
    }

    else if (vt_truck_avg < 140 && vt_truck_avg > 105){
      // if Vtruck decreases to 150V check Pdbias val by turning off one LED, waiting 50ms measuring VT1 turning LED back on and 
      // then using equation 7 to recalculate the Pdbias
      // turn the motor on to lower lift (go left) 
      motor_driver.set_state(LEFT);

    }
    
    else{
      // if Vtruck decreases from above 160V to below 155V stop raising the lift (turn motor off)
      // if truck voltage incrased from below 150V to 155V stop lowering the lift (turn motor off)
      motor_driver.set_state(OFF);

    }

  }
  
}

void display_voltage(){
  // display voltage to segment display and maybe change state (if enough time has passed)
  bool ready_to_update = true;
  
  if (ready_to_update){
    seg_disp.update_state();
  }

}

void change_segment_state(){
  // if enough time has passed, change the segment state to the current average
  bool ready_to_update = true;

  if (ready_to_update){
    seg_disp.set_display_value(vt_truck_avg);
  }

}


// ===== Helpers =====

void update_rolling_avg(double vt2_voltage){
  // updates rolling average queue based on new vt2 voltage

  if (!vt_truck_rolling_queue.isFull()){
    // if queue isnt full, just add it to queue and add divided value to queue 
    vt_truck_rolling_queue.push(&vt2_voltage);
    vt_truck_avg = vt_truck_avg + vt2_voltage/vt_truck_rolling_queue.getCount();
  
  }else{
    // queue is full, so must remove from it first to then add new voltage
    double removed_voltage; 
    vt_truck_rolling_queue.pop(&removed_voltage);
    vt_truck_rolling_queue.push(&vt2_voltage);
    vt_truck_avg = vt_truck_avg + vt2_voltage/queue_max_size - removed_voltage/queue_max_size;
  }
}

// ===== Setup Functions =====

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

void set_scalings(){
    // scaling variables: 0v = 170Vin, 130V = 0Vin
  int vin_scaleM = -8; // slope of 150V scaling
  int vin_scaleB = 170; // y-intercept of 150V scaling

  // scaling for LED current (0-1.5 amps) 
  double led_scaleM = 1.5;
  double led_scaleB = 0;

  // set scaling for Vin voltage and LED current
  vin_convert.set_scaling(vin_scaleM, vin_scaleB);
  led_driver.set_scaling(led_scaleM, led_scaleB);
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
    
    // Vt_2 = 
    curMillis = millis();
  } 

<<<<<<< HEAD
  // keep rolling 100 sample over 100ms average of Vt2 (operating input voltage)
  seg_disp.update_disp(vin_avg_scaled);

=======
>>>>>>> Src-Organized-Filestructure
}
