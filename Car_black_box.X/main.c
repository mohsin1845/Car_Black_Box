/*
 * File:   main.c
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */


#include <xc.h>
#include "main.h"
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)
/* Global Variables Declaration */
unsigned char time[9];
unsigned char clock_reg[3], speed[3]={0};
unsigned char *event[]={"ON","GN","G1","G2","G3","G4","GR"," C"};
volatile unsigned char count;
unsigned short adc_val;
unsigned char password [5] ="0000";
unsigned char control_flag=DASH_BOARD_FLAG;
extern unsigned char sec;
extern event_count;
static void init_config(void)
{
    init_i2c(100000); //100K
    init_ds1307();
    init_clcd();
    init_digital_keypad();
    init_adc();
    init_timer2();
    PEIE=1;
    GIE=1;
    /* To read the Password initially from the External EEPROM */
	int i;
	unsigned char add = 0xC8;
	for (i = 0; i < 4; i++)
	{
		password [i] = read_ext_eeprom (add++);
	}
    
}
//void store_event (unsigned int count);
void main(void)
{
     unsigned char key, reset_flag,menu_pos;
    init_config();
    while (1) 
    {
        get_time(clock_reg); // HH MM SS
        key = read_digital_keypad(STATE);
        for(int j=3000;j--;);

        if (key == SW1 && count != 7)
        {
            count = 7;
            store_event(count);
        }
        else if (key == SW2 && (count >=0 && count<6))
        {
            count++;
            store_event(count);
        }
        else if (key == SW3 && (count > 1 && count != 7))
        {
            count--;
            store_event(count);
        }

        else if ((control_flag==DASH_BOARD_FLAG) &&(key==SW4 || key==SW5))
        {
            clear_screen();
            clcd_print(" ENTER  PASSWORD ",LINE1(0));
            clcd_write(CURSOR_POS, INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            control_flag=LOGIN_FLAG;
            reset_flag=RESET_PASSWORD;
            TMR2ON = 1;   
        }
        else if(control_flag==LOGIN_MENU_FLAG && (key==SW6))
        {
            switch(menu_pos)
            {
                case 0:     //view log
                    clear_screen();
                    clcd_print("# TIME      E SP",LINE1(0));
                    control_flag=VIEW_LOG_FLAG;
                    reset_flag=VIEW_LOG_RESET;
                        break;
                case 1:     //clear log
                    clear_screen();
                    control_flag=CLEAR_LOG_FLAG;
                    reset_flag=RESET_MEMEORY;
                        break;
                case 2:   //Downnload Logs
                    clear_screen();
                    PEIE = 0;
                    GIE = 0;
                    init_uart(9600);
                    control_flag=DOWNLOAD_FLAG;
                    reset_flag=VIEW_LOG_RESET;
                    break;
            }
        }
        if(control_flag== LOGIN_MENU_FLAG && key == SW5)
        {
            sec=5;
            
            if(key==SW5 && sec>0 )
            {
                TMR2ON = 1;
            }
            
            
        }
        switch (control_flag)
        {
            case DASH_BOARD_FLAG:
                display_dash_board(event, speed);
                break;
            case LOGIN_FLAG:
                switch (login(key, reset_flag)) {

                    case RETURN_BACK:
                        control_flag = DASH_BOARD_FLAG;
                        TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        break;
                    case TASK_SUCCESS:
                        control_flag = LOGIN_MENU_FLAG;
                        reset_flag = RESET_LOGIN_MENU;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        continue;
                }
                break;
            case LOGIN_MENU_FLAG:
                menu_pos = login_menu(key, reset_flag);
                break;
            case VIEW_LOG_FLAG:
                view_log(key, reset_flag);
                break;
            case CLEAR_LOG_FLAG:
                if (clear_log(reset_flag) == TASK_SUCCESS) {
                    clear_screen();
                    control_flag = LOGIN_MENU_FLAG;
                    reset_flag = RESET_LOGIN_MENU;
                    continue;
                }
                break;

            case DOWNLOAD_FLAG:
            {
                int i = 0, j = 0;
                for (int k = 0; k < event_count; k++) // To display the Events on the Laptop.
                {
                    j = 0;
                    for (i = 0; i < 14; i++) {
                        if (i == 2 || i == 5) {
                            putchar(':');
                            
                        } else if (i == 8 || i == 11) {
                            putchar(' ');
                           
                        } else {
                            putchar(read_ext_eeprom(k * 10 + j)); // Read the Event from External EEPROM.
                            j++;
                        }
                    }
                    puts("\n\r");
                }
                clear_screen();
                clcd_print("Files Downloaded", LINE2(0));
                for (long delay = 500000; delay--;); // Use a different name for the loop variable
                clear_screen();
                TXIE = 0;
			    RCIE = 0;
                control_flag = LOGIN_MENU_FLAG;
                reset_flag = RESET_LOGIN_MENU;   
                break;
            }
        }
        reset_flag=RESET_NOTHING; 
    }
    
    return;
}

unsigned char data [11];
unsigned short count_event_flag;
/* To Store the Event on the External EEPROM */
void store_event (unsigned int count)
{
	int i = 0, j = 0;
	while (time [i] != '\0')									//To store the Time for the Event in 'data'.
	{
		if (time [i] != ':')									//Remove the ':' in Time array before storing the Values.
		{
			data [j++] = time [i];
		}
		i++;
	}

	data [j++] = event [count][0];						//To store the Event in 'data'.
	data [j++] = event [count][1];

	//data [j++] = speed[0];						//To store the Speed of the Car in 'data'.
	data [j++] = speed[0];
	data [j++] = speed[1];

	/* To store the Data extracted above on the External EEPROM */
	for (j = 0; j < 11; j++)
	{
		write_ext_eeprom ((event_count * 10 + j), data[j]);
	}

	/* If the Count of the Events is greater than 10; Overwrite the Events from the Beginning whenever the New Event is stored */
	if (event_count < 10)
    {
        event_count++;
    }
	else
	{
		event_count=0;
	}
}
