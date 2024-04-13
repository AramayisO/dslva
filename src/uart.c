#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define ENABLE_DATA_REGISTER_EMPTY_INTERUPT() UCSR0B |= (1 << UDRIE0);
char tx_buf[UART_TX_BUF_SIZE];
volatile unsigned int tx_head = 0;
volatile unsigned int tx_tail = 0;

/**
 * Initialize the UART with the specified baud rate. The data frame will be set
 * to 8 data bits, no parity bit, and 1 stop bit.
 */
void UART_init(unsigned int baud) {
    // Set baud rate
    unsigned int ubrr = (16000000 / 16 / baud) - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char) ubrr;
    
    // Set asynchronous normal mode
    UCSR0A &= ~(1 << U2X0);

    // Enable receiver and transmitter
    UCSR0B |= RXEN0;
    UCSR0B |= TXEN0;

    // Set frame format 8 data bit, no partity bit, 1 stop bit
    UCSR0B &= ~(1 << UCSZ02);
    UCSR0B |= (1 << UCSZ01);
    UCSR0B |= (1 << UCSZ00);
    UCSR0C &= ~(1 << UPM01);
    UCSR0C &= ~(1 << UPM00);
    UCSR0C &= ~(1 << USBS0);

    // Enable interrupts
    sei();
}

void UART_transmit_next_char() {
    if (tx_head != tx_tail) {
        UDR0 = tx_buf[tx_tail];
        tx_tail = (tx_tail + 1) % UART_TX_BUF_SIZE;
    }
}

/**
 *
 */
size_t UART_print(char *str) {
    size_t idx = 0;

    cli();
    while (str[idx] != '\0' && ((tx_head + 1) % UART_TX_BUF_SIZE != tx_tail)) {
        tx_buf[tx_head] = str[idx];
        tx_head = (tx_head + 1) % UART_TX_BUF_SIZE;
        idx++;
    }
    UCSR0B |= (1 << UDRIE0);
    sei();

    return idx;
}

void UART_job() {
}

ISR(USART_UDRE_vect)
{
    if ((tx_tail + 1) % UART_TX_BUF_SIZE != tx_head) {
        UDR0 = tx_buf[tx_tail];
        tx_tail = (tx_tail + 1) % UART_TX_BUF_SIZE;
    } else {
        UCSR0B |= _BV(UDRIE0);
    }
	//UCSR0B &= ~DATA_REGISTER_EMPTY_INTERRUPT; // Disables the Interrupt, uncomment for one time transmission of data
}
