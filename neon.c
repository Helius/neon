#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "uart.h"

#define SETBIT(reg,bit) (reg|=1<<bit)
#define CLRBIT(reg,bit) (reg&=~(1<<bit))


#define LED    5       //PORTB
#define LAMP0  3       //PORTD
#define LAMP1  4       //PORTD
#define WAKEUP 2       //PORTD


void gpio_init (void) {
//	SETBIT(DDRB,LED);
//	CLRBIT(PORTB,LED);
	SETBIT(DDRD,LAMP0);
	CLRBIT(PORTD,LAMP0);
	SETBIT(DDRD,LAMP1);
	CLRBIT(PORTD,LAMP1);
}

void interrupt_init() {
	//EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
	EIMSK |= (1 << INT0);     // Turns on INT0
	SETBIT(PORTD,WAKEUP);     // enable pull-up
	CLRBIT(DDRD,WAKEUP);      // set as input
}

void set_lamp (uint16_t level) {
		if (rand()%255 < level) {
//			SETBIT(PORTB,LED);
			CLRBIT(PORTD,LAMP0);
		} else {
//			CLRBIT(PORTB,LED);
			SETBIT(PORTD,LAMP0);
		}
		if (rand()%255 < level) {
			CLRBIT(PORTD,LAMP1);
		} else {
			SETBIT(PORTD,LAMP1);
		}
}

void start_ignition () {
	uint16_t ignition_level = 0;
	uint16_t light_timer = 0;
	while(ignition_level++ < 250) {
		set_lamp(ignition_level);
//		printf ("level: %d\n\r", ignition_level);
		_delay_ms (50);	
		if (ignition_level > 100) {
			ignition_level+=5;
		}
	}
	while(light_timer++ < 500) {
		set_lamp(ignition_level);
//		printf ("timer: %d\n\r", light_timer);
		_delay_ms (50);	
	}
}

void go_sleep () {
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	cli();
}

int main (void) {

	gpio_init();
//	uart_init();
	adc_init();
	interrupt_init();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	stdout = &uart_stream;
	stdin = &uart_stream;
	printf("Hi, how are you?\n\r");
	uint16_t first_adc_value = adc_get(0);
	printf("adc on 0 channel: %d\n\r", first_adc_value);
	srand (first_adc_value);
	while (1) {
		start_ignition();
		printf ("will sleep, by-by\n\r");
		CLRBIT(PORTB,LED); // turn off led
		CLRBIT(DDRD,LAMP0);
		CLRBIT(DDRD,LAMP1);
		ADCSRA = 0;// turn off ADC
		go_sleep();
		adc_init();
		gpio_init();
	}
	return 0;
}
