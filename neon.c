#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart.h"

#define SETBIT(reg,bit) (reg|=1<<bit)
#define CLRBIT(reg,bit) (reg&=~(1<<bit))


#define LED 5       //PORTB


void gpio_init (void) {
	SETBIT(DDRB,LED);
	SETBIT(PORTB,LED);
}

int main (void) {

	gpio_init();
	uart_init();
	stdout = &uart_stream;
	stdin = &uart_stream;
	printf("Hi, how are you?\n\r");
	srand (2309);
	while (0) {
	}
	return 0;
}
