#include <avr/io.h>
#include <util/delay.h>

int main() {
    DDRB |= (1 << PINB5);

    while (1) {
        PORTB |= (1 << PINB5);
        _delay_ms(100);
        PORTB &= ~(1 << PINB5);
        _delay_ms(100);
    }

    return 0;
}
