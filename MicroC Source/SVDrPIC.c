//   Servo Driver PIC16F877A Program in C
//   Written By : Ali Tivay

//   1. Reads from ADC
//   2. Adjusts PWM Duty Cycle in proportion

unsigned int temp_res;
unsigned short current_duty_1, old_duty_1;
unsigned short current_duty_2, old_duty_2;

#define PWM_FREQUENCY    1000
#define PUMP_ON_TIME     600
#define PUMP_OFF_TIME    50

//----------------------------------------------------------------------

void InitMicro() {              // Function: Initialize Device

  ADCON1 = 0x80;                // Configure analog inputs and Vref
  TRISA  = 0xFF;                // PORTA is input

  PORTB = 0;                    // set PORTB to 0
  TRISB = 0;                    // designate PORTB pins as output

  PORTD  = 0;                   // set PORTD to 0
  TRISD  = 0;                   // designate PORTB pins as output

  PORTC = 0;                    // set PORTC to 0
  TRISC = 0;                    // designate PORTC pins as output

  PWM1_Init(PWM_FREQUENCY);     // Initialize PWM1 module at 1KHz
  PWM2_Init(PWM_FREQUENCY);     // Initialize PWM2 module at 1KHz
  
  current_duty_1  = 0;            // initial value for current_duty 1
  old_duty_1      = 0;            // initial value for old_duty 1
  current_duty_2  = 0;            // initial value for current_duty 2
  old_duty_2      = 0;            // initial value for old_duty 2
  temp_res        = 0;            // initial value for temp_res
}

//----------------------------------------------------------------------

void ChargePump() {

  PORTB = 0b00000000;
  Delay_us(PUMP_OFF_TIME);
  
  PORTB = 0b00001111;
  Delay_us(PUMP_ON_TIME);
  
  PORTB = 0b00000000;
  Delay_us(PUMP_OFF_TIME);
  
  PORTB = 0b11110000;
  Delay_us(PUMP_ON_TIME);
}

//----------------------------------------------------------------------

void ReadToPWM() {

  temp_res = ADC_Read(1);         // Get 10-bit results of AD conversion

  if (temp_res < 512) {
    current_duty_1 = (512 - temp_res) / 2;
    current_duty_2 = 0;
  } else {
    current_duty_1 = 0;
    current_duty_2 = (temp_res - 512) / 2;
  }

  if (old_duty_1 != current_duty_1) {  // if change in duty cycle requested
    PWM1_Change_Duty(current_duty_1);  //    set new duty ratio,
    old_duty_1 = current_duty_1;       //    memorize it
  }

  if (old_duty_2 != current_duty_2) {  // if change in duty cycle requested
    PWM2_Change_Duty(current_duty_2);  //    set new duty ratio,
    old_duty_2 = current_duty_2;       //    memorize it
  }
}

//----------------------------------------------------------------------

void main() {

  InitMicro();
  PWM1_Start();                   // start PWM1
  PWM2_Start();                   // start PWM2

  do {                            // Infinite Loop
    ChargePump();
    ReadToPWM();
  } while(1);
}
