#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include <util/delay.h>

int main()
{
	sei();
	uart_init(9600);
	while(1) {
		uart_print("tak\n\r");
		_delay_ms(1000);
	}
}
