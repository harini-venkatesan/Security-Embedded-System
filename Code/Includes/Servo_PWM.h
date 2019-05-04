#ifndef SERVO_PWM_H_
#define SERVO_PWM_H_


#include <avr/io.h>
// NOTE*** THIS NEW CODE TARGETS PB6 NOT PB3

void Wait()
{
	uint8_t i;
	for(i=0;i<50;i++)
	{
		_delay_loop_2(0);
		_delay_loop_2(0);
		_delay_loop_2(0);
	}

}

void servo_set_PWM(int cycle) {
	OCR1A = cycle;
	//Wait();
	

}

void servo_PWM_on() {
//	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
	//TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10;
	//ICR1=4999;  //fPWM=50Hz (Period = 20ms Standard).
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)

	ICR1=4999;  //fPWM=50Hz (Period = 20ms Standard).

	DDRD|=(1<<PORTD4)|(1<<PORTD5);   //PWM Pins as Out
	
	//servo_set_PWM(1000);

}

void servo_PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}



#endif // SERVO_PWM_H_