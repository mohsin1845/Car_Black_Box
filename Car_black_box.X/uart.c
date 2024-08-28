/*
 * File:   uart.c
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */
#include <xc.h>
#include "main.h"
#include "uart.h"

void init_uart(unsigned long baud)
{
    /* Serial initialization */
	RX_PIN = 1;
	TX_PIN = 0;
    
	/* Clock selection control bit */
	CSRC = 0;
	/* 9bit TX enable or disable bit */
	TX9 = 0;
	/* UART Tarsmition enable bit */
	TXEN = 1;
	/* Synchronous or Asynchronous mode selection */
	/* Asynchronous */
	SYNC = 0;
	/* Low or High baud rate selection bit */
	/* High Baud Rate */
	BRGH = 1;
	/* Trasmiter Shift register status bit */
	TRMT = 0;
	/* 9th bit trasmite data */
	TX9D = 0;


	/* TX/RC7 and RX/RC6 act as serial port */
	SPEN = 1;
	/* 9bit RX enable or disable bit */
	RX9 = 0;
	/* Single reception enable or disable */
	SREN = 0;
	/* Continous reception enable or disable */
	CREN = 1;
	/* Adress detection enable bit */
	ADDEN = 0;
	/* Frame error bit */
	FERR = 0;
	/* Overrun error bit */
	OERR = 0;
	/* 9th bit received bit */
	RX9D = 0;
    /* Baud Rate Setting Register */
    SPBRG = (FOSC / (16 * baud)) - 1;
    /* TX interrupt enable bit */
	TXIE = 1;
	/* TX interrupt flag bit */
	TXIF = 0;
	/* RX interrupt enable bit */
	RCIE = 1;
	/* RX interrupt enable bit */
	RCIF = 0;

}

unsigned char getchar(void)
{
    /* Wait for the byte to be received */
    while (RCIF != 1)
    {
        continue;
    }
    
    /* Clear the interrupt flag */
    RCIF = 0;
    
    /* Return the data to the caller */
    return RCREG;
}

void putchar(unsigned char data)
{
    /* Transmit the data to the Serial Port */
    TXREG = data;
    
    /* Wait till the transmission is complete */
    do
    {
        continue;
    } while (TXIF != 1);
    
    /* Clear the interrupt flag */
    TXIF = 0;
}

void puts(const char *s)
{
    while (*s)
    {
        putchar(*s++);
    }
}