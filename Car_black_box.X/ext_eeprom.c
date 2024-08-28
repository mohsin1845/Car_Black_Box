/*
 * File:   ext_eeprom.c
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */

#include "ext_eeprom.h"
#include <xc.h>
#include "i2c.h"

void write_ext_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();                       // Start condition
    i2c_write(SLAVE_WRITE_EXT);       // Send slave address with write bit
    i2c_write(address);               // Send memory address
    i2c_write(data);                  // Write data
    i2c_stop();                        // Stop condition
}

unsigned char read_ext_eeprom(unsigned char address)
{
    unsigned char data;
    
    i2c_start();                       // Start condition
    i2c_write(SLAVE_WRITE_EXT);       // Send slave address with write bit
    i2c_write(address);               // Send memory address
    i2c_rep_start();                  // Repeated start condition
    i2c_write(SLAVE_READ_EXT);        // Send slave address with read bit
    data = i2c_read(0);               // Read data
    i2c_stop();                        // Stop condition

    return data;
}
