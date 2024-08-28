/*
 * File:   ds1307.h
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */

#ifndef DS1307_H
#define	DS1307_H

#define SLAVE_WRITE             0b11010000 // 0xD0
#define SLAVE_READ              0b11010001 // 0xD1

#define SEC_ADDR                0x00
#define MIN_ADDR                0x01
#define HOUR_ADDR               0x02


void init_ds1307(void);
unsigned char read_ds1307(unsigned char addr);
void write_ds1307(unsigned char addr, unsigned char data);
void display_time(unsigned char *clock_reg);
void get_time(unsigned char *clock_reg);

#endif	/* DS1307_H */



