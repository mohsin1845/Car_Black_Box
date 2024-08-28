#include <xc.h>
#include "i2c.h"
#include "ext_eeprom.h"

// I2C Initialization Function
void init_i2c(unsigned long baud)
{
    SSPCON = 0x28; // I2C Master mode
    SSPCON2 = 0x00; // Disable I2C features
    SSPADD = (unsigned char)(FOSC / (4 * baud)) - 1; // Set baud rate
    SSPEN = 1; // Enable I2C
}

static void i2c_wait_for_idle(void)
{
    while (R_nW || (SSPCON2 & 0x1F)); // Wait until the bus is idle
}

void i2c_start(void)
{
    i2c_wait_for_idle();
    SEN = 1; // Send start condition
}

void i2c_stop(void)
{
    i2c_wait_for_idle();
    PEN = 1; // Send stop condition
}

void i2c_rep_start(void)
{
    i2c_stop();
    i2c_start();
}

unsigned char i2c_read(unsigned char ack)
{
    unsigned char data;
    i2c_wait_for_idle();
    RCEN = 1; // Enable reception
    i2c_wait_for_idle();
    data = SSPBUF; // Read data from SSPBUF
    ACKDT = ack; // Set ACK/NACK
    ACKEN = 1; // Send ACK/NACK
    return data;
}

int i2c_write(unsigned char data)
{
    i2c_wait_for_idle();
    SSPBUF = data; // Write data to SSPBUF
    return !ACKSTAT; // Return acknowledgment status
}
