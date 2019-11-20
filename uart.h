#ifndef UART_H
#define UART_H

#define UART_RX_BUFFER_SIZE      (128)

void uart_init(unsigned baudrate);
void uart_putchar(char c);
void uart_print(const char *string);
void uart_printf(const char *format, ...);
void uart_getline(char *line);
int uart_is_line_ready(void);

#endif /* UART_H */
