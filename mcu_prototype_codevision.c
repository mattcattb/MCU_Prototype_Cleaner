/*******************************************************************************
This program was created by the CodeWizardAVR V4.00a
Automatic Program Generator
? Copyright 1998-2023 Pavel Haiduc, HP InfoTech S.R.L.
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
#include <controllers.c>

// 7-seg display
#define Seg1 PORTB.0
#define Seg2 PORTB.1
#define Seg3 PORTB.2
#define SegData PORTD

// helper functions
void motor_loop(float In150V_Val); 
void Show_Value (unsigned int In);
void steady_voltage();

// global IN150v values
unsigned long AveInPout150V = 0;
unsigned int InPout150V = 0;
unsigned int FreeCount = 0;

// constant n_eff, Voltage out
const float n_eff = 0.7;
const float Vt_out = 24;
float Vlm_0 = 0;
 
unsigned char error_state = 1; // error state starts in boot up (1)
unsigned char DISPLAY_Counter = 0; // what digit to display
unsigned char DISPLAY[4]; // digits for display
unsigned char SegmentData[] =
{
    // 0 ,  1  ,  2  ,  3  , 4  , 5  , 6  , 7  , 8  , 9  ,off , C  , P  , t  , o  , O  , 15
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x39, 0x73, 0x78, 0x5C, 0x3F,
    // N , F  ,  y  , E  ,   r  , L  , n  , -  , H  , u  , U  , A  , S  , 8. , 0. , 1. , 31
    0x37, 0x71, 0x6E, 0x79, 0x50, 0x38, 0x54, 0x40, 0x76, 0x1C, 0x3E, 0x77, 0x6D, 0xFF, 0xBF, 0x86,
    // 2., 3. , 4. , 5. , 6. , 7. , 8. , 9. , D  , M  , q  , ?  , '  , .  , b  , d  , 47
    0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF, 0x3F, 0x55, 0x67, 0x53, 0x20, 0x80, 0x7C, 0X5E,
    //    y   ,     o
    0x6A, 0x10, 0x63, 0x00
};

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    float n = 300;
    unsigned char dot_mask = 0b10000000;
    // here display 1 of the digits and add to the sum to calculate the average over 300 readings 
    Seg1 = 1;
    Seg2 = 1;
    Seg3 = 1;             
    
    if(DISPLAY_Counter == 0)
        {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
        SegData = SegmentData[DISPLAY[1]];   
        // mask data to turn on dot 
        SegData = SegData | dot_mask;
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
    // AveInPout150V += read_vin_volt();                         
    // this should properly scale between 100-200 volts 
    AveInPout150V += (((read_adc(Sens150Vin)*100.0)/1024.0) + 100);
                 
    
    if(FreeCount++ >= n)
    {
        // calculate average once n readings have been reached
        FreeCount = 0;
        InPout150V = (AveInPout150V/n);
        AveInPout150V=0;    
    } 
    
    //------------------------------ 
}

void main(void)
{
    // power variables
    float Pd_bias;
    float Pd_LEDL;
    float Pd_LEDR;

    // voltage of truck
    float Vt_truck;

    // resistance of line 
    float R_Line_val; 

    #include <Init.c>
                          
    delay_ms(5*1000); 
    
    control_LED(ON, ON);            
    
    // configure voltage calculations for R_line, Pd_LEDL, Pd_LEDR, Pd_bias
    // voltage_calc_phase(&R_Line_val, &Pd_LEDL, &Pd_LEDR, &Pd_bias);

    control_LED(ON, ON);
              
    // wait for first voltage to be between 140 and 160
    steady_voltage();

    // set both drivers low and enable on 
    M_D_R = 0;
    M_D_L = 0;
    M_EN = 0;

    while (1)
    {

        check_error_state();
        
        //! make this more accurate!
        float temp_val = (((read_adc(Sens150Vin)*100.0)/1024.0) + 100);
      
        // set value to be shown on 7-seg display
        Show_Value(temp_val); 

        // control motor based on reading
        motor_loop(temp_val);

    }
}


void check_error_state(){
    // make changes to error state 
    
    if (InPout150V < 132 && (M_D_L == 1 && M_EN == 1))
    {                    
        // voltage below 132V, and lift lowering 
        error_state = 2; 
    }else
    {
        // no error state
        error_state = 0;
    }
}

void steady_voltage()
{
    // wait for voltage to steady out to being between 160 and 140
    
    while(InPout150V>160 || InPout150V < 140){
        // keep waiting while greater then 160 or less then 140
        delay_ms(2);
    }
    // finally, should be between 160 and 140 so return
    return;
}

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

void motor_loop(float In150V_Val)
{
    // control motor according to the In150V_Val
    if(In150V_Val > 160)
    {            
        // lift UP 
        Motor_R_L_Off(Right);// Right Left OFF 
        // cheek motor curent
    }

    else if((In150V_Val < 140) && (In150V_Val > 105))
    {                                          
        // lift DOWN
        Motor_R_L_Off(Left);// Right Left OFF 
        
        if (In150V_Val < 132){
            // turn off one headlamp until lift stops moving
            control_LED(ON, OFF);
        } 
    }

    else
    {         
        // lift STOP
        control_LED(ON,ON);    

        Motor_R_L_Off(OFF);// Right Left OFF 
    }
}
