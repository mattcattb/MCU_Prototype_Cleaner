#define ON  1
#define OFF 0

#define Right  1 // up
#define Up    1

#define Left   2 // down
#define Down 2

// vin
#define Sens150Vin  PORTC.0

// LED
#define IN0_LED PORTB.3
#define IN1_LED PORTB.4
#define SEL_LED PORTB.5
#define EN_LED  PORTC.2
#define SENSE_LED PORTC.1
  
// Motor
#define M_D_R  PORTC.3
#define M_D_L  PORTC.4
#define M_EN   PORTC.5
#define M_R_Sense ADC.6 
//! #define Sens_Motor  6


// ADC Voltage Reference: Int., cap. on AREF
#define ADC_VREF_TYPE ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

void Motor_R_L_Off (unsigned char Motor);
void control_LED (unsigned char Led1, unsigned char Led2);
unsigned int read_adc(unsigned char adc_input);
float read_vin_volt();
float avg_read_vin_volt(int n);
float get_SENSE_led();


void Motor_R_L_Off (unsigned char Motor)// 2 = left(down), 1 = Right(up),  0 = OFF
{
    if(Motor == Right)
    {             
        // up
        M_D_L = 0;
        M_D_R = 1;
        M_EN  = 1;
    }
    else if(Motor == Left)
    {   
        // down
        M_D_L = 1;
        M_D_R = 0;
        M_EN  = 1;
    }
    else
    {
        M_D_L = 0;
        M_D_R = 0;
        M_EN  = 0;
    }
}

int get_motor_state()
{ 
    // 2 down, 1 up, 0 off
    if (M_D_L == 1 && M_D_R == 0){
        return Left;
    }
    else if (M_D_L == 0 && M_D_R == 1){
        return Right;
    }
    else{
        return 0;
    }
}

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
    // 200 to 100 voltage reading

    float scale_m = 20;
    float scale_b = 100;
    // read vin voltage
    float vin = read_adc(Sens150Vin);
    // scale vin voltage
    vin = (vin/1023.0)*5.0; // make a 0-5V reading
    vin = (vin * scale_m) + scale_b; // scale between 100-200V
    return vin;
}

float avg_read_vin_volt(int n)
{
    // read vin voltage as average over n times
    float sum_vin = 0;     
    int i = 0;
    while (i < n){
        sum_vin += read_vin_volt();
        i += 1;
    }

    return sum_vin/n;
}

float get_SENSE_led()
{
    // 1A == 4.7V
    // 0A == 0V

    const float max_current = 1; // max current in amps for scaling 
    const float max_voltage = 4.7;
    float current;                                      
    
    // read and scale sense voltage 
    float sense_voltage = read_adc(SENSE_LED); // 10bit reading
    sense_voltage = (sense_voltage/1024.0)*5; // make a 0-5 volts reading
    if (sense_voltage > max_voltage){
        sense_voltage = max_voltage; // max voltage
    }

    current = (sense_voltage/max_voltage)*max_current; // scale to current
    
    return current;
}