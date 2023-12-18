/*******************************************************************************
This program was created by the CodeWizardAVR V4.00a
Automatic Program Generator
© Copyright 1998-2023 Pavel Haiduc, HP InfoTech S.R.L.
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
// I/O Registers definitions
#include <mega328p.h>
// Delay functions
#include <delay.h>
unsigned int read_adc(unsigned char adc_input);
//=============================================================================
#define ON  1
#define OFF 0

#define Right  1
#define Left   2

#define Sens150Vin  0
#define Sens_LEDS   1
#define Sens_Motor  6


#define Seg1 PORTB.0
#define Seg2 PORTB.1
#define Seg3 PORTB.2

#define SegData PORTD

#define IN0_LED PORTB.3
#define IN1_LED PORTB.4
#define SEL_LED PORTB.5
#define EN_LED  PORTC.2

#define M_D_R  PORTC.3
#define M_D_L  PORTC.4
#define M_EN   PORTC.5

//=============================================================================
unsigned long AveInPout150V = 0;
unsigned int InPout150V = 123;
unsigned int FreeCount = 0;
//unsigned int FreeCount1 = 0, FreeCount2 = 0;


unsigned char DISPLAY_Counter = 0;
unsigned char DISPLAY[4];
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
//=============================================================================
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
void LEDS_On_Off (unsigned char Led1, unsigned char Led2) // 1 = ON  0 = OFF
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
// Read the AD conversion result
// Read Voltage=read_adc*(Vref/1024.0)
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
//=============================================================================
//=============================================================================
void main(void)
{
#include <Init.c>

	LEDS_On_Off(ON, ON);

	while (1)
		{
      //Show_Value(InPout150V);

      //Show_Value(read_adc(Sens150Vin));
      //Show_Value(read_adc(Sens_LEDS));
      //Show_Value(read_adc(Sens_Motor));

      //Show_Value(((read_adc(Sens150Vin) / 10) + 100)); 
    
    	//InPout150V = ((read_adc(Sens150Vin)/10)+100);
      
      Show_Value(InPout150V); 
      
      if(InPout150V > 160)
       {
        Motor_R_L_Off(Right);// Right Left OFF 
        // cheek motor curent
       }
      
      else if((InPout150V < 140) && (InPout150V > 105))
       {
        Motor_R_L_Off(Left);// Right Left OFF 
       }
      
      else
       {
        Motor_R_L_Off(OFF);// Right Left OFF 
       }

		}
}
//=============================================================================
//=============================================================================
