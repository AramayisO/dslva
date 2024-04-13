#include <avr/io.h>
#include <util/delay.h>

void UART_Init(unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char) ubrr;
    
    // Set asynchronous normal mode
    UCSR0A &= ~(1 << U2X0);

    // Enable receiver and transmitter
    UCSR0B |= RXEN0;
    UCSR0B |= TXEN0;

    // Set frame format 8 data bit, no partity bit 1 stop bit
    UCSR0B &= ~(1 << UCSZ02);
    UCSR0B |= (1 << UCSZ01);
    UCSR0B |= (1 << UCSZ00);
    UCSR0C &= ~(1 << UPM01);
    UCSR0C &= ~(1 << UPM00);
    UCSR0C &= ~(1 << USBS0);
}

void UART_Transmit_Char(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))) {
        // wait for transmit buffer to be empty
    }
    UDR0 = data;
}

int main() {

    unsigned int ubrr = (16000000 / 16 / 9600) - 1;

    UART_Init(ubrr);

    DDRB |= (1 << PINB5);

    while (1) {
        PORTB |= (1 << PINB5);
        UART_Transmit_Char('O');
        UART_Transmit_Char('N');
        UART_Transmit_Char('\n');
        UART_Transmit_Char('\r');
        _delay_ms(100);
        PORTB &= ~(1 << PINB5);
        UART_Transmit_Char('O');
        UART_Transmit_Char('F');
        UART_Transmit_Char('F');
        UART_Transmit_Char('\n');
        UART_Transmit_Char('\r');
        _delay_ms(100);
    }

    return 0;
}
