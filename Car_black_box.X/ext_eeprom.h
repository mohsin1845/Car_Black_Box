/*
 * File:   ext_eeprom.h
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */
#ifndef EXT_EEPROM_H
#define EXT_EEPROM_H

/* MACRO for the Slave Address */
#define SLAVE_READ_EXT		0xA1
#define SLAVE_WRITE_EXT		0xA0

/* Function Declarations */
void write_ext_eeprom (unsigned char address, unsigned char data);
unsigned char read_ext_eeprom (unsigned char address);
void store_event (unsigned int count);

#endif
