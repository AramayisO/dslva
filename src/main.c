#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"
#include "circular_buffer.h"

int main()
{
    UART_init(9600);

    DDRB |= (1 << PINB5);
    PORTB &= ~(1 << PINB5);

    char str[] = "Hello, world!\n";
    UART_puts(str, strlen(str));

    while (1)
    {
        // PORTB |= (1 << PINB5);
        // _delay_ms(1000);
        // PORTB &= ~(1 << PINB5);
        // _delay_ms(1000);
    }

    return 0;
}
