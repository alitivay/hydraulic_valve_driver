#line 1 "C:/Users/AliTY/Desktop/SVDrPIC/SVDrPIC.c"






unsigned int temp_res;
unsigned short current_duty_1, old_duty_1;
unsigned short current_duty_2, old_duty_2;







void InitMicro() {

 ADCON1 = 0x80;
 TRISA = 0xFF;

 PORTB = 0;
 TRISB = 0;

 PORTD = 0;
 TRISD = 0;

 PORTC = 0;
 TRISC = 0;

 PWM1_Init( 1000 );
 PWM2_Init( 1000 );

 current_duty_1 = 0;
 old_duty_1 = 0;
 current_duty_2 = 0;
 old_duty_2 = 0;
 temp_res = 0;
}



void ChargePump() {

 PORTB = 0b00000000;
 Delay_us( 50 );

 PORTB = 0b00001111;
 Delay_us( 600 );

 PORTB = 0b00000000;
 Delay_us( 50 );

 PORTB = 0b11110000;
 Delay_us( 600  - 210);
}



void ReadToPWM() {

 temp_res = ADC_Read(1);

 if (temp_res < 512) {
 current_duty_1 = (512 - temp_res) / 2;
 current_duty_2 = 0;
 } else {
 current_duty_1 = 0;
 current_duty_2 = (temp_res - 512) / 2;
 }

 if (old_duty_1 != current_duty_1) {
 PWM1_Change_Duty(current_duty_1);
 old_duty_1 = current_duty_1;
 }

 if (old_duty_2 != current_duty_2) {
 PWM2_Change_Duty(current_duty_2);
 old_duty_2 = current_duty_2;
 }
}



void main() {

 InitMicro();
 PWM1_Start();
 PWM2_Start();

 do {
 ChargePump();
 ReadToPWM();
 } while(1);
}
