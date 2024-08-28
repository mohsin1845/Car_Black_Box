/*
 * File:   adc.h
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */

#ifndef ADC_H
#define	ADC_H
#define CHANNEL0  0x00
void init_adc(void);
unsigned short read_adc(unsigned char );
void display(unsigned short );

#endif	/* ADC_H */

