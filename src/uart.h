#ifndef __DSLVA_UART_H__
#define __DSLVA_UART_H__

#include <stdlib.h>

#define UART_TX_BUF_SIZE 512
#define UART_RX_BUF_SIZE 512

void UART_init(unsigned int baud);

size_t UART_print(char *str);

#endif
