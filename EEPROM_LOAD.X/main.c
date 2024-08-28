/* 
 * File:   main.c
 * Author: MOSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:24 PM
 * Description  : To initially Load the Default Password in the External EEPROM for the Car Black Box to access while Execution.
*/

#include <xc.h>
#include "i2c.h"
#include "ext_eeprom.h"

void main(void)
{
    init_i2c(100000); // Initialize I2C with 100kHz baud rate
    
    // Write password '1000' to EEPROM
    write_ext_eeprom (0xC8, '1');
	write_ext_eeprom (0xC9, '0');
	write_ext_eeprom (0xCA, '0');
	write_ext_eeprom (0xCB, '0');
    while (1)
    {
        // Main loop
    }
}


