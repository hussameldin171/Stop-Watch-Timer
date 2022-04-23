

/*******************************************PROGRAM OVERVIEW***************************************************/

/* This program is designed for controlling a stop-watch system, It uses the idea of creating a structure consisting of 3
   values: hours , minutes and seconds.

   Each member is 8 bits wide, and is treated as a BCD number, such that if the first four bits exceeds the value of 9(1001),
   we increase it by 6.


   Ex: if the seconds member is equal to 9 (00001001) and is incremented to be 10, adding 10 to 6 we get:

   	 	 	 	 	 	 	 	 	 	 	 0000 1010
   	 	 	 	 	 	 	 	 	 	 +	 0000 0110
   	 	 	 	 	 	 	 	 	 	 _______________
   	 	 	 	 	 	 	 	 	 	     0001 0000      =  0x10

   	So the first 4 bits are the tens digit of the member , and the other 4 bits represents its unit digit and each of them
   	will be displayed on a separate seven segment :)
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
#include "timer.h"

//Creating an instance of struct time initialized to zeros:

struct Time myTime = {0,0,0};

//Creating an array of pointers to the three members of myTime to simplify the implementation of displaying time:
unsigned char *arr[] = {&(myTime.hours) , &(myTime.minutes) ,&(myTime.seconds)};

/*********************************TIMER 1 FUNCTIONS***************************************************/

//1_Initialization:
void timer1_COMP_init(void) {

	TCCR1A = (1<<FOC1A); //Timer1 A is a non PWM mode

	TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10); //Using Compare Mode and a prescaler of 64

	TCNT1 = 0 ; //initializing the timer with zeros

	OCR1A = 15625; //Setting the compare value to count 1 second

	TIMSK|=(1<<OCIE1A); //Enabling Compare A interrupt

}

//2_Interrupt Service Routine:
ISR(TIMER1_COMPA_vect) {
	//incrementing the Time structure using BCD method and checking if a member's value reaches 60:
	time_inc(&myTime);


}

//3-Disable Timer by Disabling the clock
void timer_off(){

	TCCR1B&=~ 0x03;

}

//4-Enable Timer
void timer_on(){

	TCCR1B|=0x03;

}

/********************************EXTERNAL INTERRUPT 0 : TO RESET THE TIMER******************************************************/

//1_Initialization:
void INT0_init() {
	GICR|=(1<<INT0); //Enable module interrupt

	MCUCR|=(1<<ISC01); //Sensitive to falling edge

	DDRD&=~(1<<PD2); //PD2 is input

	PORTD|=(1<<PD2);//Internal pull up resistance on


}
//2_Interrupt Service Routine:
ISR(INT0_vect){
	//Clear myTime and the timer register
	myTime.hours = 0 ;
	myTime.minutes = 0 ;
	myTime.seconds = 0 ;
	TCNT1 =0;
}


/***********************************EXTERNAL INTERRUPT 1 : STOP THE TIMER**************************************************/
//1_Initialization:
void INT1_init(){
	GICR|=(1<<INT1);//Enable module interrupt

	MCUCR|= 0x0C; //Sensitive to rising edge

	DDRD&=~(1<<PD3); //PD3 is input
}
//2_Interrupt Service Routine: timer off
ISR(INT1_vect){
	timer_off();
}

/***********************************EXTERNAL INTERRUPT 1 : RESUME THE TIMER**************************************************/
//1_Initialization:

void INT2_init(){

	GICR|=(1<<INT2);//Enable module interrupt

	DDRB&=~(1<<PB2);//PB2 is input

	PORTB|=(1<<PB2);//Internal pull up resistance on

}
//2_Interrupt Service Routine: timer on
ISR(INT2_vect){
	timer_on();
}

/*******************************************DISPLAY FUNCTION*******************************************************************/
void display2(struct Time *t) {

	//TASK : PERFORM A ROLL FORM A0 TO A6 AND WRITE THE REQUIRED VALUES IN THEM
	PORTA = 0x01;
	//Looping on each member of myTime
	for(int i = 0 ; i < 3 ; i++) {
		//Display the tens digit of member i
		PORTC = upper( *arr[i] );
		_delay_ms(2);
		PORTA<<=1;
		//display the units digit of member i
		PORTC = lower( *arr[i] );
		_delay_ms(2);
		PORTA<<=1;
	}

}

/*******************************************MAIN FUNCTION*******************************************************************/


int main(void) {

	DDRC|=0x0F;//Enable the first 4 pins of PORTC as outputs
	PORTC = 0; //Initial value = 0 ;

	DDRA|=0x3F;//Enable the first6 pins of PORTA as outputs
	PORTA = 0x01 ; //Initial value = 1 ;

	SREG|=(1<<7);//Enable I bit

	timer1_COMP_init(); //Timer Initialization
	INT0_init();// External Interrupt 0 Initialization
	INT1_init();// External Interrupt 1 Initialization
	INT2_init();// External Interrupt 2 Initialization
	timer_on(); //Set timer on




	while(1) {
		display2(&myTime); //Display thr value of each member of myTime on each 7 segment using multiplexing technique
	}

}
