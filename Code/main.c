#include <avr/io.h>
#include <avr/interrupt.h>
#include <scheduler.h>
#include <keypad.h>
#include <timer.h>
#include <io.h>
#include <io.c>
#include <util/delay.h>
#include "speaker.h"
#include "usart.h"
#include "Servo_PWM.h"

//Task that performs locking and unlocking mechanisms. This is where a password is entered, which is checked by the state machine
//If the password is correct, it either locks or unlocks the door 
//which is defined using servo motor turning using Servo_PWM

enum SM_Lock_states{Lock_start, Locked_Lock, Lock_lock_wait, Lock_unlock, Lock_unlock_wait, Lock_wait}Lock_state;
char passCode[6] = {'#','1','2','3','4','#'};
unsigned int Count = 0;

void SM_Lock_Tick(){
	unsigned char x = GetKeypadKey();
	switch(Lock_state){		
		case Lock_start:
			Count = 0;
			Lock_state = Lock_unlock;
			break;
		
		case Locked_Lock:
			if(x == '\0'){
				Lock_state = Locked_Lock;
			}
			else{
				Lock_state = Lock_lock_wait;
				if(x==passCode[Count]){
				
					LCD_Cursor(17);
					LCD_WriteData(x);
				
					Count++;
				}
				else{
					
					LCD_ClearScreen();
				
					Count = 0;
				}
			}
			break;
		
		case Lock_lock_wait:
			if(x != '\0'){
				Lock_state = Lock_lock_wait;
			}
			else if(x == '\0' && Count<=5){
				Lock_state = Locked_Lock;
			}
			else if(x == '\0' && Count > 5){
				LCD_ClearScreen();
				LCD_Cursor(17);
				LCD_DisplayString(1, "Unlocked");
			
				_delay_ms(1000);
			
				LCD_ClearScreen();
			
				servo_set_PWM(316);
		
				Lock_state = Lock_unlock;
				Count = 0;
			}
			break;
		
		case Lock_unlock:
			if(x == '\0'){
				Lock_state = Lock_unlock;
			}
		
			else{
				Lock_state = Lock_unlock_wait;
				if(x==passCode[Count]){
					LCD_Cursor(17);
					LCD_WriteData(x);
				
				
					Count++;
				}
				else{
					LCD_ClearScreen();
					LCD_DisplayString(1, "Wrong Password!!");
					Count = 0;
				}
			}
			break;
		
		case Lock_unlock_wait:
			if(x != '\0'){
				Lock_state = Lock_unlock_wait;
			}
			else if(x == '\0' && Count<=5){
				Lock_state = Lock_unlock;
			}
			else if(x == '\0' && Count > 5){
			
			
				Lock_state = Lock_wait;
				Count = 0;
			
			
			}
			break;
		case Lock_wait:
			if(Count <= 5){
				LCD_ClearScreen();
			
				LCD_DisplayString(1,"Locking");
				Lock_state = Lock_wait;
			
			
				Count++;
			
				servo_set_PWM(535);
		
			}
			else{
				LCD_ClearScreen();
				Lock_state = Locked_Lock;
				Count = 0;
	
			}
			break;
	}
	switch(Lock_state){		
		case Lock_start:
			break;
	
		case Locked_Lock:
			LCD_DisplayString(1, "Locked");
		
			break;
		
		case Lock_lock_wait:
			break;
		
		case Lock_unlock:
			LCD_DisplayString(1, "Unlocked");
		
			break;
		
		case Lock_unlock_wait:
			break;
		case Lock_wait:
			break;
	}
}

//This state machine task senses if the motion detector detected any motion. The PIR motion detector is hooked up to PORTB which detects motion and toggles the sensor check on 

enum Motion_Sensor_States{motion_sensor_off, motion_sensor_on}motion_sensor_state;

unsigned char motion_sensor_check = 0x00;

void SM_Motion_Tick(){
	motion_sensor_check = PINB & 0x01;
	
	switch(motion_sensor_state){		
		case motion_sensor_off:
			if(motion_sensor_check){
				motion_sensor_state = motion_sensor_on;
			}
			else
			motion_sensor_state = motion_sensor_off;
			break;
		case motion_sensor_on:
			if(motion_sensor_check ){
				motion_sensor_state = motion_sensor_on;
			}
			else
			motion_sensor_state = motion_sensor_off;
			break;
	}
	switch(motion_sensor_state){		
		case motion_sensor_off:
			break;
		case motion_sensor_on:
			break;
	}
}


//This state machine task deals with what happens once motion is detected.
//It sends out alarm signal using speaker and LED light connected 
//It also emits bluetooth signal using USART to send "Break IN!!!"

enum SM_Detect_States{detect_on, detect_emit, detect_off}detect_state;
unsigned char count = 0x00;


void SM_Detect_Tick(){
	switch(detect_state){		
		case detect_off:
			if((Lock_state == Locked_Lock || Lock_state == Lock_lock_wait) && motion_sensor_state == motion_sensor_on){
				detect_state = detect_on;
			}
			else
			detect_state = detect_off;
			break;
		case detect_on:
			if(Lock_state == Lock_unlock){
				detect_state = detect_off;
			}
			else if(count <= 10){
				detect_state = detect_on;
				count++;
			}
			else if(count > 10){
				detect_state = detect_emit;
				count = 0;
			}
			break;
		case detect_emit:
			if(Lock_state == Lock_unlock){
				detect_state = detect_off;
			}
			else if(count <= 10){
				detect_state = detect_emit;
				count++;
			}
			else if(count > 10){
				detect_state = detect_on;
				count = 0;
			}
			break;
		
		
	}

	switch(detect_state){		
		case detect_off:
			speaker_set_PWM(0);
			PORTB = 0x00;
		
			break;
		
		case detect_on:
		
			speaker_set_PWM(100);
			PORTB = 0xFF;
			if(USART_IsSendReady(0)){
				USART_SendString("Break in!! \n");
			}
			break;
		
		case detect_emit:
			speaker_set_PWM(0);
			PORTB = 0x00;
			break;
	}
}




int main(void)
{
	
	//Initialize all ports 
	
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x0A; PORTB = 0xF5;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD |= 0xFF; PORTD = 0x00;

	//Initialize all components - LCD display, servo motor, speaker, bluetooth 
	servo_PWM_on();
	speaker_PWM_on();
	initUSART(0);
	LCD_init();
	
	//Set timer 
	TimerOn();
	TimerSet(50);
	
	LCD_ClearScreen();
	LCD_Cursor(0);
	
	//Initialize start states
	Lock_state = Lock_start;
	motion_sensor_state = motion_sensor_off;
	detect_state = detect_off;
	
	
	//run state machines 
	while (1)
	{
		SM_Lock_Tick();
		SM_Motion_Tick();
		SM_Detect_Tick();

		while(!TimerFlag){}
		TimerFlag=0;
		
	}
	
	
	
}