/*	Author: Mayur Ryali
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Extra Credit #2  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif


enum States {Start, Run, Alarm, Release, Wait, Stop} state;

unsigned char tempB; //track which state we are in
unsigned char snooze; //snooze button (A0)
unsigned char stop; //stop button (A1)
unsigned long countTime = 0;
unsigned long cntr = 0;
unsigned char i = 0;
unsigned char s = 0; //tracks if stop or snooze was pressed

void Tick() {
	switch(state) {
		case Start:
			state = Run;
			break;
		case Run:
			if (cntr < countTime) { //alarm time not reached
				cntr++;
				state = Run;
			}
			else if (cntr == countTime) {
				i = 0;
				state = Alarm;
			}
			break;
		case Alarm:
			if (i < 20) { //ring for some time
				i++;
				state = Alarm;
			}
			else if (snooze) { //snooze
				i = 0;
				s = 1;
				state = Release;
			}
			else if (stop) { //if stop is pressed
				s = 2;
				i = 0;
				cntr = 0;
				state = Release;
			}
			else if (i == 20) {
				i = 0;
				state = Wait;
			}
			break;
		case Release: 
			if (!snooze && s == 1) {
				s = 0;
				state = Wait;
			}
			else if (!stop && s == 2) {
				s = 0;
				state = Stop;
			}
			else {
				state = Release;
			}
			break;
		case Wait:
			if (i < 40) { //wait some time
				i++;
				state = Wait;
			} 
			else if (stop) {
				i = 0;
				cntr = 0;
				s = 2;
				state = Release;
			}
			else if (i == 40) { //time reached
				i = 0;
				cntr = 0;
				state = Alarm;
			}
			break;
		case Stop:
			state = Run;
			break;

		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			break;
		case Run:
			tempB = 0x01; //signifies which state the system is in
			break;
		case Alarm:
			tempB = 0x02;
			break;
		case Release:
			//tempB = 0x03;
			break;
		case Wait:
			tempB = 0x04;
			break;
		case Stop:
			tempB = 0x05;
			break;
		default:
			break;
	}
	
}



int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output

	TimerSet(100); //1000ms
	TimerOn();

	state = Start;
	countTime = 100;
	i = 0;

    	while (1) {
		snooze = ~PINA & 0x01;
		stop = ~PINA & 0x02;

		Tick();

		while(!TimerFlag) {}
		TimerFlag = 0;

		PORTB = tempB; //Led signify which state the alarm is in
    	}
    	return 1;
}
