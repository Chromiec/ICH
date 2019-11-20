#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "uart.h"

char buff[256];
int kol=0;
void uart_init(unsigned baudrate)
{
    /* Enable TX and RX. No interrupts for now */
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0); //to samo co (1<<RXCIE0)

    /* Enable odd parity bit, 8-bit frame */
    UCSR0C = _BV(UPM01) | _BV(UPM00) | _BV(UCSZ01) | _BV(UCSZ00);

    /* Baudrate setting */
    UBRR0 = F_CPU/((uint32_t)16*baudrate);
}

void uart_putchar(char c)
{
    /* Loop until there a empty space for the next character */
    while (!(UCSR0A & _BV(UDRE0)));

    /* Place next character in the buffer */
    UDR0 = c;
}

void uart_print(const char *string)
{
	int i = 0;
	kol = 0;
	while(*string != '\0')
	{
		buff[i] = *string;
		string++;		
		i++;
	}
	buff[i+1] = '\0';
	UCSR0B |= (1<<UDRE0);//wywolanie przerwania
}

void uart_printf(const char *format, ...)
{
    char buffer[256];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    uart_print(buffer);
}

struct uart_rx_buffer {
    char line[UART_RX_BUFFER_SIZE];
    unsigned curr;
};

static volatile struct uart_rx_buffer uart_line_buffer;
static volatile int uart_line_ready = 0;

ISR(USART_UDRE_vect)
{
	UDR0 = buff[kol];
	kol++;
	if(buff[kol] == '\0')
	{	
		UCSR0B &= ~(1<<UDRE0);
	}
}

ISR(USART_RX_vect)
{
    uint8_t c = UDR0;

    /* If there already an unprocessed line in the buffer drop the character */
    if (uart_line_ready)
        return;

    if (c == '\n' || c == '\r') {
        uart_line_buffer.line[uart_line_buffer.curr] = '\0';

        /* WARNING!!! uart_print in it's current form is based on busy waiting
         * and therefore shouldn't be used in interrupts. A solution here would
         * be to write interrupt-based UART transmitter as well ;) */
        uart_print("\n\r");

        /* Drop empty lines */
        if (uart_line_buffer.curr == 0)
            return;

        uart_line_ready = 1;
        uart_line_buffer.curr = 0;

    } else {
        uart_line_buffer.line[uart_line_buffer.curr++] = c;

        /* WARNING!!! uart_putchar in it's current form is based on busy waiting
         * and therefore shouldn't be used in interrupts. A solution here would
         * be to write interrupt-based UART transmitter as well ;) */
        uart_putchar(c);
    }
}

void uart_getline(char *line)
{
    while (!uart_line_ready);
    strcpy(line, (const char *)uart_line_buffer.line);
    uart_line_ready = 0;
}

int uart_is_line_ready(void)
{
    return uart_line_ready;
}
