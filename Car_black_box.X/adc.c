#include <xc.h>
#include "clcd.h"

void init_adc(void)
{
    /* Selecting Right Justification */
    ADFM = 1;
    
    /* Starting the ADC Module */
    ADON = 1;
}

unsigned short read_adc(unsigned char channel)
{
    unsigned short adc_reg_val; // 2bytes
    ADCON0=(ADCON0 &0XC7) | (channel << 3);
    
    /* Start the ADC conversion */
    GO = 1;
    
    /* Wait for the conversion to complete */
    while (nDONE == 1); //nDONE = 0 GO = 0
    
   
    adc_reg_val = (ADRESH << 8) | ADRESL;  // 11 1111 1111 -> 1023
    
    return adc_reg_val; // 0 to 1023
    
}
/*void display(unsigned short speed_val)
{
    char buff[3];
    int i=2;
    do
    {
        buff[i]=(speed_val %10) + '0';
        speed_val=speed_val/10;
    }while(i--);
    buff[3]='\0';
    clcd_print(buff,LINE2(14));
    
    
}*/
