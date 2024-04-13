#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

int main() {
    UART_init(9600);

    DDRB |= (1 << PINB5);

    UART_print("hello world\r\n");
    while (1) {
        UART_print("Blink\r\n");
        PORTB |= (1 << PINB5);
        _delay_ms(100);
        PORTB &= ~(1 << PINB5);
        _delay_ms(100);
    }

    return 0;
}
