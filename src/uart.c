#include "uart.h"
#include "circular_buffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <assert.h>

#define ENABLE_DATA_REGISTER_EMPTY_INTERUPT() UCSR0B |= (1 << UDRIE0);

char tx_backing_buffer[UART_TX_BUF_SIZE];
char rx_backing_buffer[UART_RX_BUF_SIZE];
circular_buffer_t tx_buf;
circular_buffer_t rx_buf;
circular_buffer_handle_t tx_buf_handle = &tx_buf;
circular_buffer_handle_t rx_buf_handle = &rx_buf;

void _try_transmit_next_char();

/**
 * Initialize the UART with the specified baud rate. The data frame will be set
 * to 8 data bits, no parity bit, and 1 stop bit.
 */
void UART_init(unsigned int baud)
{
    // Set baud rate
    unsigned int ubrr = (16000000 / 16 / baud) - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

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

    // Set up circular buffers
    circular_buffer_init(tx_buf_handle, tx_backing_buffer, UART_TX_BUF_SIZE);
    circular_buffer_init(rx_buf_handle, rx_backing_buffer, UART_RX_BUF_SIZE);
}

size_t UART_puts(char *str, size_t len)
{
    assert(str != NULL);
    assert(len > 0);
    size_t num_written;
    if ((num_written = circular_buffer_puts(tx_buf_handle, str, len)))
    {
        _try_transmit_next_char();
    }
    return num_written;
}

ISR(USART_UDRE_vect)
{
    _try_transmit_next_char();
}

void _try_transmit_next_char()
{
    // Only write if the data register is empty and there is data to write
    if (UCSR0A & _BV(UDRE0))
    {
        char data;
        if (circular_buffer_get(tx_buf_handle, &data))
        {
            UDR0 = data;
            UCSR0B |= _BV(UDRIE0);
        }
        else
        {
            // If there is no data to write, disable the uart data register empty
            // interrupt otherwise a new interrupt will be triggered immediately
            // after the interrupt route terminates.
            UCSR0B &= ~_BV(UDRIE0);
        }
    } else {
        // If the data register is not empty, enable the interrupt to write the
        // next character when the data register is empty.
        UCSR0B |= _BV(UDRIE0);
    }
}
