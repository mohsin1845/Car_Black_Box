/*
 * File:   uart.h
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */

#ifndef UART_H
#define	UART_H

//#define FOSC                20000000 
/* MACRO for the PORTS */
#define RX_PIN					TRISC7
#define TX_PIN					TRISC6


void init_uart(unsigned long baud);
unsigned char getchar(void);
void putchar(unsigned char data);
void puts(const char *s);

#endif	/* UART_H */