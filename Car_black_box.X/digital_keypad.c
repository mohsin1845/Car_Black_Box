/*
 * File:   digital_keypad.c
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */
#include <xc.h>
#include "i2c.h"
#include "ds1307.h"
#include "clcd.h"
#include "digital_keypad.h"


void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            
            return KEYPAD_PORT & INPUT_LINES;
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    
    return ALL_RELEASED;
}
