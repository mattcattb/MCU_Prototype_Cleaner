/*******************************************************************************
This program was created by the CodeWizardAVR V4.00a
Automatic Program Generator
� Copyright 1998-2023 Pavel Haiduc, HP InfoTech S.R.L.
http://www.hpinfotech.ro

Project : IECLLC_P2_1
Version : 1.1.1
Date    : 12/14/2023
Author  : Abbas

Chip type               : ATmega328P
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************************************/

/*
	TODO
	- use sense to calculate R_line, Pd_LEDL, Pd_LEDR, Pd_bias
	- setup rolling average (using queue)
	- possibly add timing checks for vin 
	- maybe seperate functions into other files

*/

// I/O Registers definitions
#include <mega328p.h>
// Delay functions
#include <delay.h>

// helper functions
unsigned int read_adc(unsigned char adc_input);
void voltage_calc_phase(double *R_line, double *Pd_LEDL, double *Pd_LEDR, double *Pd_bias);

void motor_loop(double In150V_Val); // 

// circuit functions
void control_LED (unsigned char Led1, unsigned char Led2); // 1 = ON  0 = OFF
void Show_Value (unsigned int In);
void Motor_R_L_Off (unsigned char Motor);
float read_vin_volt();

// equations functions
double no_load_V_truck(double Vt_0, double Pd_bias, double R_line);
double one_load_V_truck(double Vt_1, double Pd_bias, double Pd_LEDL, double R_line);
double two_load_V_truck(double Vt_2, double Pd_bias, double Pd_LEDL, double Pd_LEDR, double R_line);

double Pd_bias_one_two(double Vt0, double Vt1, double Pd_LEDL, double R_line);

double Pd_bias_one_three(double Vt0, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line);
double Pd_bias_two_three(double Vt1, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line);

double R_LINE_EQ_One(double vt_0, double vt_1, double Pd_LEDL, double Pd_bias);
double R_LINE_EQ_Two(double vt_0, double vt_1, double vt_2, double Pd_LEDL, double Pd_LEDR);

//=============================================================================
#define ON  1
#define OFF 0

#define Right  1
#define Left   2

// vin
#define Sens150Vin  0
#define Sens_LEDS   1
#define Sens_Motor  6

// 7-seg display
#define Seg1 PORTB.0
#define Seg2 PORTB.1
#define Seg3 PORTB.2

#define SegData PORTD

// LED
#define IN0_LED PORTB.3
#define IN1_LED PORTB.4
#define SEL_LED PORTB.5
#define EN_LED  PORTC.2

// Motor
#define M_D_R  PORTC.3
#define M_D_L  PORTC.4
#define M_EN   PORTC.5

//=============================================================================
unsigned long AveInPout150V = 0;
unsigned int InPout150V = 123;
unsigned int FreeCount = 0;
//unsigned int FreeCount1 = 0, FreeCount2 = 0;

// constant n_eff, Voltage out
const double n_eff = 0.7;
const double Vt_out = 24;

double zero_buffer = 0.001;  // add to denominator to make sure never divide 0

unsigned char DISPLAY_Counter = 0; // what digit to display
unsigned char DISPLAY[4]; // digits for display
unsigned char SegmentData[] =
{
	// 0 , 1  , 2  , 3  , 4  , 5  , 6  , 7  , 8  , 9  ,off , C  , P  , t  , o  , O  , 15
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x39, 0x73, 0x78, 0x5C, 0x3F,
	// N , F  , y  , E  , r  , L  , n  , -  , H  , u  , U  , A  , S  , 8. , 0. , 1. , 31
	0x37, 0x71, 0x6E, 0x79, 0x50, 0x38, 0x54, 0x40, 0x76, 0x1C, 0x3E, 0x77, 0x6D, 0xFF, 0xBF, 0x86,
	// 2., 3. , 4. , 5. , 6. , 7. , 8. , 9. , D  , M  , q  , ?  , '  , .  , b  , d  , 47
	0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF, 0x3F, 0x55, 0x67, 0x53, 0x20, 0x80, 0x7C, 0X5E,
	//	y   ,     o
	0x6A, 0x10, 0x63, 0x00
};


//=============================================================================
// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	Seg1 = 1;
	Seg2 = 1;
	Seg3 = 1;
	if(DISPLAY_Counter == 0)
		{
		SegData = SegmentData[DISPLAY[1]];
		DISPLAY_Counter++;
		Seg3 = 0;
		}
	else if( DISPLAY_Counter == 1)
		{
		SegData = SegmentData[DISPLAY[2]];
		DISPLAY_Counter++;
		Seg2 = 0;
		}
	else
		{
		SegData = SegmentData[DISPLAY[3]];
		DISPLAY_Counter = 0;
		Seg1 = 0;
		}
	//------------------------------ 
  AveInPout150V += ((read_adc(Sens150Vin)/10)+100);
  
	if(FreeCount++ >= 300)
		{
      FreeCount = 0;
      InPout150V = (AveInPout150V/300);
      AveInPout150V=0;
    
//			if(FreeCount2 == 0){LEDS_On_Off(OFF, OFF); FreeCount2 = 2; }
//			else {LEDS_On_Off(ON, ON); FreeCount2 = 0; }
    } 
    
	//------------------------------ 
}
//=============================================================================
// ADC Voltage Reference: Int., cap. on AREF
#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

//=============================================================================
void main(void)
{
#include <Init.c>

	// power variables
	double Pd_bias;
	double Pd_LEDL;
	double Pd_LEDR;

	// voltage of truck
	double Vt_truck;

	// resistance of line 
	double R_Line; 

	// configure voltage calculations for R_line, Pd_LEDL, Pd_LEDR, Pd_bias
	calculate_voltages(&R_Line, &Pd_LEDL, &Pd_LEDR, &Pd_bias);

	// turn both LEDs on 
	control_LED(ON, ON);

	while (1)
	{
      //Show_Value(InPout150V);

      //Show_Value(read_adc(Sens150Vin));
      //Show_Value(read_adc(Sens_LEDS));
      //Show_Value(read_adc(Sens_Motor));

      //Show_Value(((read_adc(Sens150Vin) / 10) + 100)); 
    
	  // get reading from ADC
	  InPout150V = read_vin_volt(vin_scaleM, vin_scaleB);
      
	  // calculate truck voltage from ADC reading
	  double truck_voltage = calculate_truck_voltage(InPout150V, R_Line, Pd_LEDL, Pd_LEDR, Pd_bias);

	  // set value to be shown on 7-seg display
	  Show_Value(truck_voltage); 

	  // control motor based on reading
      motor_loop(truck_voltage);

	}
}
//=============================================================================
//=============================================================================

double calculate_truck_voltage(double vin_150v, double R_Line, double Pd_LEDL, double Pd_LEDR, double Pd_bias)
{
	// use the vin_150v and variables to determine the trucks voltage given 
	double vt_truck;

	// determine which equation to use based on what LEDs turned on 
	if (IN0_LED == 1 && IN1_LED == 1){
		// both LEDs are on (vt2 equation)
		vt_truck = two_load_V_truck(vin_150v, Pd_bias, Pd_LEDL, Pd_LEDR, R_Line);

	}else if (IN0_LED == 1 || IN1_LED == 1){
		// one of the LEDs are on (vt1 equation)
		vt_truck = one_load_V_truck(vin_150v, Pd_bias, Pd_LEDL, R_Line);
	}else{
		// no LEDs are on (vt0 equation)
		vt_truck = no_load_V_truck(vin_150v, Pd_bias, R_Line);
	}

	return vt_truck;
}


void voltage_calc_phase(double *R_line, double *Pd_LEDL, double *Pd_LEDR, double *Pd_bias)
{
	// calculate R_line, Vt_truck, Pd_LEDL, Pd_LEDR

	// measure vt0 through averaging 100 samples
	double Vt_0 = read_vin_volt(100);

	// turn left LED light and wait 30 ms
	control_LED(1, 0);
	delay_us(30);

	// measure Vt1 and I_LED.left over 100 samples avg
	double Vt_1 = read_vin_volt(100);
	// todo set I_LED.left to 1A... ?
	//! double I_LEDL = led_driver.read(100);

	// turn right LED on (both on) and wait 50 ms
	control_LED(1, 1);
	delay_us(50);

	// measure Vt2 and I_LED.right over 200 samples avg
	double Vt_2 = read_vin_volt(200);
	//! double I_LEDR = led_driver.read(200);

	// calculate PdLEDL, PdLEDR, PdBias
	*Pd_LEDL = (I_LEDL*Vt_out)/n_eff;
	*Pd_LEDR = (I_LEDR*Vt_out)/n_eff;

	// using equation 8 calculate RLine and store as constant 
	R_Line = R_LINE_EQ_Two(Vt_0, Vt_1, Vt_2, Pd_LEDL, Pd_LEDR);

	// using equation 7 calculate external bias power pdbias and store as semi-constant variable
	Pd_bias = Pd_bias_one_two(Vt_0, Vt_1, Pd_LEDL, R_Line);

	// using equation 3 measure truck voltage Vtruck
	Vt_truck = two_load_V_truck(Vt_2, Pd_bias, Pd_LEDL, Pd_LEDR, R_Line);

	//todo keep rolling 100 sample over 100ms average of Vt2 (operating input voltage)

	//! keep rolling 100 sample over 100ms average of Vt2 (operating input voltage)

}

void Motor_R_L_Off (unsigned char Motor)// 1 = ON  0 = OFF
{
	if(Motor == Right)
		{
		M_D_L = 0;
		M_D_R = 1;
		M_EN  = 1;
		}
	else if(Motor == Left)
		{
		M_D_R = 0;
		M_D_L = 1;
		M_EN  = 1;
		}
	else
		{
		M_D_R = 0;
		M_D_L = 0;
		M_EN  = 0;
		}
}
//=============================================================================
void control_LED (unsigned char Led1, unsigned char Led2) // 1 = ON  0 = OFF
{
	if(Led1 == ON)
		IN0_LED = ON;
	else
		IN0_LED = OFF;

	if(Led2 == ON)
		IN1_LED = ON;
	else
		IN1_LED = OFF;

	SEL_LED = 1;
	EN_LED  = 1;
}
//=============================================================================
void Show_Value (unsigned int In)
{
	// display integer on 3 digit 7-segment display
	if(InPout150V <= 100)
	{
		DISPLAY[1] = 19;//Err
		DISPLAY[2] = 20;
		DISPLAY[3] = 20;
	}
	else
	{ 
		DISPLAY[1] = ((In / 100) % 10);
		DISPLAY[2] = ((In / 10) % 10);
		DISPLAY[3] = ((In / 1) % 10);
	}
}


unsigned int read_adc(unsigned char adc_input)
{
	ADMUX = adc_input | ADC_VREF_TYPE;
	// Delay needed for the stabilization of the ADC input voltage
	delay_us(10);
	// Start the AD conversion
	ADCSRA |= (1 << ADSC);
	// Wait for the AD conversion to complete
	while ((ADCSRA & (1 << ADIF)) == 0);
	ADCSRA |= (1 << ADIF);
	return ADCW;
}

float read_vin_volt()
{
	// read single vin voltage

	float scale_m = -8;
	float scale_b = 170;
	// read vin voltage
	float vin = read_adc(Sens150Vin);
	// scale vin voltage
	vin = (vin/1024.0)*5.0; // make a 0-5V reading
	vin = (vin * scale_m) + scale_b; // scale between 0-150V
	return vin;
}

float read_vin_volt(int n)
{
	// read vin voltage as average over n times
	float sum_vin = 0;
	for(int i = 0; i < n; i++)
	{
		sum_vin += read_vin_volt();
	}

	return sum_vin/n;
}

void motor_loop(double In150V_Val)
{
	// control motor according to the In150V_Val
	if(In150V_Val > 160)
	{
		Motor_R_L_Off(Right);// Right Left OFF 
		// cheek motor curent
	}

	else if((In150V_Val < 140) && (In150V_Val > 105))
	{
		Motor_R_L_Off(Left);// Right Left OFF 
	}

	else
	{
		Motor_R_L_Off(OFF);// Right Left OFF 
	}
}

// ======================== Electrical Equations ==============================

// ====== Voltage Truck Equations ========

double no_load_V_truck(double Vt_0, double Pd_bias, double R_line){
    return (Vt_0 + (Pd_bias/(Vt_0 + zero_buffer))*R_line);
}

double one_load_V_truck(double Vt_1, double Pd_bias, double Pd_LEDL, double R_line){
    return (Vt_1 + ((Pd_LEDL + Pd_bias)*R_line)/(Vt_1+zero_buffer));
}

double two_load_V_truck(double Vt_2, double Pd_bias, double Pd_LEDL, double Pd_LEDR, double R_line){
    return (Vt_2 + (Pd_LEDL + Pd_LEDR + Pd_bias)*R_line/(Vt_2 + zero_buffer));
}

// ====== Power bias Equations

double Pd_bias_one_two(double Vt0, double Vt1, double Pd_LEDL, double R_line){
    // setting no load voltage truck equation equal to 1 load voltage truck to get Pd
    // pdbias = ((Vt0 - Vt1)/Rline - PdLED.l/Vt1)*(Vt0 * Vt1)/(Vt0 - Vt1) 

    double val_1 = (Vt0 - Vt1)/(R_line + zero_buffer);
    double val_2 = Pd_LEDL/(Vt1 + zero_buffer);
    double val_3 = (Vt0*Vt1)/(Vt0 - Vt1 + zero_buffer);

    return (val_1 - val_2)*val_3;
}

double Pd_bias_one_three(double Vt0, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line){
    // setting no load voltage truck equation equal to 2 load voltage truck

    double val1 = (Vt0 - Vt2)/(R_line + zero_buffer);
    double val2 = (Pd_LEDL + Pd_LEDR)/(Vt2 + zero_buffer);
    double val3 = (Vt0 * Vt2)/(Vt0 - Vt2 + zero_buffer);

    return (val1 - val2)*val3;
}

double Pd_bias_two_three(double Vt1, double Vt2, double Pd_LEDL, double Pd_LEDR, double R_line){
    // setting 1 load voltage truck equal to 2 load voltage truck equation

}

// ====== Resistance Line Equations

double R_LINE_EQ_One(double vt_0, double vt_1, double Pd_LEDL, double Pd_bias){
    // RLine = (Vt0 * VT1)/(PdLED.L * Vt0/(Vt0-Vt1) + Pdbias)
    return (vt_0 * vt_1)/(Pd_LEDL * vt_0/(vt_0-vt_1 + zero_buffer) + Pd_bias + zero_buffer);
}

double R_LINE_EQ_Two(double vt_0, double vt_1, double vt_2, double Pd_LEDL, double Pd_LEDR){
    // vt0, vt1, vt2, PdL, PdR
    double val1 = vt_2 - vt_1;
    double val2 = (Pd_LEDL + Pd_LEDR)/(vt_0-vt_2 + zero_buffer);
    double val3 = (Pd_LEDL)/(vt_0-vt_1 + zero_buffer);

    double rline = val1/(val2 - val3 + zero_buffer);
    return rline;
}