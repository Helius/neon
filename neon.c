#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"

#define SETBIT(reg,bit) (reg|=1<<bit)
#define CLRBIT(reg,bit) (reg&=~(1<<bit))


#define LED   5       //PORTB
#define LAMP0 3       //PORTB
#define LAMP1 4       //PORTB


void gpio_init (void) {
	SETBIT(DDRB,LED);
	CLRBIT(PORTB,LED);
	SETBIT(DDRD,LAMP0);
	CLRBIT(PORTD,LAMP0);
	SETBIT(DDRD,LAMP1);
	CLRBIT(PORTD,LAMP1);
}

int main (void) {

	gpio_init();
	uart_init();
	adc_init();
	stdout = &uart_stream;
	stdin = &uart_stream;
	printf("Hi, how are you?\n\r");
	uint16_t first_adc_value = adc_get(0);
	printf("adc on 0 channel: %d\n\r", first_adc_value);
	srand (first_adc_value);
	uint8_t ignition_level = 0;
	while (1) {
		if (ignition_level < 150) {
			ignition_level++;
		}
		if (rand()%155 < ignition_level) {
			SETBIT(PORTB,LED);
			CLRBIT(PORTD,LAMP0);
		} else {
			CLRBIT(PORTB,LED);
			SETBIT(PORTD,LAMP0);
		}
		if (rand()%155 < ignition_level) {
			CLRBIT(PORTD,LAMP1);
		} else {
			SETBIT(PORTD,LAMP1);
		}
		printf ("cnt: %d\n\r", ignition_level);
		_delay_ms (100);	
	}
	return 0;
}
