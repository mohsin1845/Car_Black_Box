/*
 * File:   car_black_def.c
 * Author: MOHSIN MOHI UD DIN
 * Created on 4 August, 2024, 2:23 PM
 */

#include "main.h"

extern unsigned char count;
extern unsigned short adc_val;
extern unsigned char clock_reg[3];
extern unsigned char password [5];
extern unsigned char control_flag;
int event_count=0;
extern unsigned char sec;
extern unsigned char return_time;
char *menu[]={"View log","Clear log","Download"};


void display_dash_board(char event[], unsigned char speed[])
{
    
      clcd_print(" TIME     EV  SP", LINE1(0));
      clcd_print(event[count],LINE2(10));
      __delay_ms(100);
        //Read speed
      adc_val=read_adc(CHANNEL0)/10.23;
       
       speed[0]=(adc_val/100) + 48;
       speed[0]=((adc_val/10)%10) + 48;
       speed[1]=(adc_val%10) + 48;
       clcd_print(speed,LINE2(14));
       display_time(clock_reg); // HH:MM:SS --> 13:14:15
}

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
   __delay_us(100);
}

char login(unsigned char key,unsigned char reset_flag)
{
    static char npassword[4];
    static unsigned char attempt_rem,i;
    char spassword[4];
    if(reset_flag==RESET_PASSWORD)
    {
        return_time=5;
        attempt_rem='3';
        i=0;
        npassword[0]='\0';
        npassword[1]='\0';
        npassword[2]='\0';
        npassword[3]='\0';
        key=ALL_RELEASED;
        
        
    }
    if(return_time==0)
    {
        return RETURN_BACK;
    }
    if (key==SW4 && i<4) //SW4 ->'4'
    {
        
        npassword[i]='1';
        clcd_putch('*',LINE2(6 + i));
        i++;
        return_time=5;
    }
    else if(key==SW5 && i<4) //SW5 ->'2'
    {
        npassword[i]='0';
        clcd_putch('*',LINE2(6+i));
        i++;
        return_time=5;
    }
    if(i==4)
    {
        for (int j=0;j<4;j++)
        {
            unsigned char add = 0xC8;  //starting address where password is stored in External EEPROM 
            spassword[j]=password[j];
             
        }
        if (strncmp(npassword, password,4)==0)
        {
            //Menu Screen
            return TASK_SUCCESS;
        }
        else //wrong Password
        {
            attempt_rem--;
            if(attempt_rem=='0')
            {
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print(" You are blocked",LINE1(0));
                clcd_print("Wait...for 60sec",LINE2(0));
                sec=60;
                while(sec!=0)
                {
                    clcd_putch((sec/10) + '0', LINE2(11));
                    clcd_putch((sec%10) + '0', LINE2(12));
                }
                attempt_rem='3';
            }
            else
            {
                clear_screen();
                //prompt wronf Password 3sec
                clcd_print(" WRONG PASSWORD ",LINE1(0));
                clcd_putch(attempt_rem,LINE2(0));
                clcd_print(" attempt remain",LINE2(1));
                __delay_ms(3000);
            }
            clear_screen();
            clcd_print(" ENTER  PASSWORD ",LINE1(0));
            clcd_write(CURSOR_POS, INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            i=0;
            return_time=5;
        } 
    }
    return 0x10;
}


char login_menu(unsigned char key, unsigned char reset_flag)
{
    static char menu_pos;
    if (reset_flag == RESET_LOGIN_MENU)
    {
        menu_pos = 0;
        clear_screen();  // Clear the screen when the menu is reset
    }

    if (key == SW5 && menu_pos < 2)  // Change the condition to < 3 to include all menu options
    {
        menu_pos++;
        clear_screen();
    }
    else if (key == SW4 && menu_pos > 0)
    {
        menu_pos--;
        clear_screen();
    }

    if (menu_pos == 0)
    {
        clcd_putch('>', LINE1(0));
        clcd_print(menu[menu_pos], LINE1(2));
        clcd_print(menu[menu_pos + 1], LINE2(2));
    }
    else if (menu_pos == 1)
    {
        clcd_print(menu[menu_pos - 1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
        clcd_putch('>', LINE2(0));
    }
    else if (menu_pos == 2)
    {
        clcd_print(menu[menu_pos - 1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
        clcd_putch('>', LINE2(0));
    }
    if (key == SW3)
    {
        clear_screen();
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
        control_flag = DASH_BOARD_FLAG;
        reset_flag = RESET_PASSWORD;
    }
    return menu_pos;  // Return the current menu position
}
void view_log(unsigned char key, unsigned char reset_flag) {
    char rlog[11];
    unsigned char add;
    static unsigned char j=0;
    static unsigned char rpos = 0; // Ensure rpos is initialized
    // Handle case with no logs
    if (event_count == 0) {
        clcd_print(" No logs", LINE2(0));
    } else {
        
        // Reset rpos if requested
        if (reset_flag == VIEW_LOG_RESET) {
            rpos = 0;
        }

        // Navigate through events
        if (key == SW5 && rpos < (event_count - 1)) {
            rpos++;
        } else if (key == SW4 && rpos > 0) {
            rpos--;
        }
        if(rpos>9)
        {
            rpos=0;
        }

        // Calculate EEPROM address
        
        add = (rpos * 10);

        // Read 10 bytes from EEPROM
        for (int i = 0; i < 10; i++) {
            rlog[i] = read_ext_eeprom(add + i);
        }

        // Display data on the CLCD
        clcd_putch(rpos % 10 + '0', LINE2(0)); // Display the event index

        clcd_putch(rlog[0], LINE2(2));
        clcd_putch(rlog[1], LINE2(3));
        clcd_putch(':', LINE2(4));

        clcd_putch(rlog[2], LINE2(5));
        clcd_putch(rlog[3], LINE2(6));
        clcd_putch(':', LINE2(7));

        clcd_putch(rlog[4], LINE2(8));
        clcd_putch(rlog[5], LINE2(9));

        clcd_putch(rlog[6], LINE2(11));
        clcd_putch(rlog[7], LINE2(12));

        clcd_putch(rlog[8], LINE2(14));
        clcd_putch(rlog[9], LINE2(15));
    }
    if (key == SW3) {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            control_flag = LOGIN_MENU_FLAG;
            reset_flag = RESET_LOGIN_MENU;
            rpos = 0; 
            return;
    }
}

char clear_log(unsigned char reset_flag)
{


    if(control_flag==CLEAR_LOG_FLAG)
    {
         event_count=0;
        clcd_print(" Logs Cleared ",LINE1(0));
        //clcd_print(" Successfully ",LINE2(0));
        __delay_ms(3000);
        return TASK_SUCCESS;
    }
    return TASK_FAIL;
}
