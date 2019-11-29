#include "tm4c123gh6pm.h"
#include <string.h>
#include "pad.h"

#define sw2 0x01
#define sw1 0x10
#define RED (1U << 1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void blink(int duration) // duration = 10 * number of seconds to blink
{
    int d = duration;
    while(d--)
    {
        GPIO_PORTF_DATA_R |= RED;

                while (!(NVIC_ST_CTRL_R & 0x10000))
                {
                }

                GPIO_PORTF_DATA_R &= ~RED;
    }
}

void delayUs(int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3; j++)
        {
        }
}

void delayMilli(int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++)
        {
        }
}

void init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; //PORT A
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //PORT B

    GPIO_PORTA_DEN_R |= 0x1C;
    GPIO_PORTA_DIR_R |= 0x1C;

    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DIR_R |= 0xFF;

    //SYSTICK TIMER

    //////////////////////
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0xFF;
    GPIO_PORTF_DEN_R |= (sw1 | sw2);
    GPIO_PORTF_DIR_R &= ~(sw1 | sw2);
    GPIO_PORTF_PUR_R |= (sw1 | sw2);

    ///////////////////////
}

void init_timer() {
    NVIC_ST_CTRL_R = 0x0;
    NVIC_ST_CURRENT_R |= 0X0;
    NVIC_ST_RELOAD_R = 160000 - 1;
    NVIC_ST_CTRL_R = 0x5;
}
////////////////////////////////////////////////////////
/////////////////////////LCD////////////////////////////
////////////////////////////////////////////////////////
void init_LCD(void)
{
    delayMilli(20);
    lcd_command(0x30);
    delayMilli(5);
    lcd_command(0x30);
    delayUs(50);
    lcd_command(0x30);
    delayUs(50);
    lcd_command(0x38);
    lcd_command(0x06);
    lcd_command(0x01);
    lcd_command(0x0F);
}

void lcd_command(unsigned char command)
{
    GPIO_PORTA_DATA_R = 0x00;
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R |= 0x10;
    delayUs(1);
    GPIO_PORTA_DATA_R = 0x00;

    if (command < 4)
    {
        delayMilli(2);
    }
    else
    {
        delayUs(40);
    }
}

void lcd_data(unsigned char data)
{
    GPIO_PORTA_DATA_R = 0x4;
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R = 0x14;
    delayUs(1);
    GPIO_PORTA_DATA_R = 0x00;
    delayUs(40);
}

void write_lcd(unsigned char *data, short length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        lcd_data(data[i]);
        delayMilli(1);
    }
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
init_keypad()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; //PORT C
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; //PORT E

    GPIO_PORTC_DEN_R |= columns;
    GPIO_PORTC_DIR_R &= ~columns;
    GPIO_PORTC_PUR_R |= columns;

    GPIO_PORTE_ODR_R |= rows;
    GPIO_PORTE_DEN_R |= rows;
    GPIO_PORTE_DIR_R |= rows;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
int main(void)
{
    init();
    init_LCD();
    init_keypad();

    int sw1_on, sw2_off;
    int seconds = 0;
    int ten_millis = 0;
    char counterStr[7];
    int strLen = 0;


    while (1)
    {
        sw1_on = !(GPIO_PORTF_DATA_R & sw1);
        sw2_off = (GPIO_PORTF_DATA_R & sw2);
        //input loop
        if (poll_keypad()) //if a key is pressed
        {
            unsigned int key_pressed = get_key();

            if (strLen < 4 && key_pressed < 10) //if pressed key is a valid input and input is still not a 4 digit number
            {
                lcd_command(0x01);
                char c = key_pressed + '0';
                counterStr[strLen] = c;
                strLen++;

                seconds = seconds * 10 + (int)key_pressed;
                write_lcd(counterStr, strLen);
                write_lcd(".00", 3);
            }
        }

        // excution loop
        if (strLen == 4 || sw1_on) //if 4 digit number or left switch is pressed
        {
        	init_timer();
            while (sw2_off && (seconds || ten_millis)) // rigth key not pressed or counter value != 0
            {
                lcd_command(0x80);

				if(!ten_millis) {
					seconds--;
					ten_millis=99;
				} else {
					ten_millis--;
				}
                
                counterStr[0] = (char)(seconds % 10000 / 1000);
                counterStr[1] = (char)(seconds % 1000 / 100);
                counterStr[2] = (char)(seconds % 100 / 10);
                counterStr[3] = (char)(seconds % 10 / 1);
                counterStr[4] = '.';
                counterStr[5] = (char)(ten_millis / 10);
                counterStr[6] = (char)(ten_millis % 10);

                write_lcd(counterStr, 7);

                while (!(NVIC_ST_CTRL_R & 0x10000));

                sw2_off = (GPIO_PORTF_DATA_R & sw2);

            }
            if (!seconds)
            {
                write_lcd("countdown end", 13);
                blink(50);
                lcd_command(0x01);
            }
        }
    }
    return 0;
}
