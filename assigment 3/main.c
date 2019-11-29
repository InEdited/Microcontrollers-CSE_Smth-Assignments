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

#define MAX_PRECISION   (10)
static const double rounders[MAX_PRECISION + 1] =
{
    0.5,                // 0
    0.05,               // 1
    0.005,              // 2
    0.0005,             // 3
    0.00005,            // 4
    0.000005,           // 5
    0.0000005,          // 6
    0.00000005,         // 7
    0.000000005,        // 8
    0.0000000005,       // 9
    0.00000000005       // 10
};

char* ftoa(double f, char* buf, int precision)
{
    char* ptr = buf;
    char* p = ptr;
    char* p1;
    char c;
    long intPart;

    // check precision bounds
    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;

    // sign stuff
    if (f < 0)
    {
        f = -f;
        *ptr++ = '-';
    }

    if (precision < 0)  // negative precision == automatic precision guess
    {
        if (f < 1.0) precision = 6;
        else if (f < 10.0) precision = 5;
        else if (f < 100.0) precision = 4;
        else if (f < 1000.0) precision = 3;
        else if (f < 10000.0) precision = 2;
        else if (f < 100000.0) precision = 1;
        else precision = 0;
    }

    // round value according the precision
    if (precision)
        f += rounders[precision];

    // integer part...
    intPart = f;
    f -= intPart;

    if (!intPart)
        *ptr++ = '0';
    else
    {
        // save start pointer
        p = ptr;

        // convert (reverse order)
        while (intPart)
        {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }

        // save end pos
        p1 = p;

        // reverse result
        while (p > ptr)
        {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }

        // restore end pos
        ptr = p1;
    }

    // decimal part
    if (precision)
    {
        // place decimal point
        *ptr++ = '.';

        // convert
        while (precision--)
        {
            f *= 10.0;
            c = f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }

    // terminating zero
    *ptr = 0;

    return buf;
}
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
    NVIC_ST_CTRL_R = 0x0;
    NVIC_ST_CURRENT_R |= 0X0;
    NVIC_ST_RELOAD_R = 160000 - 1;
    NVIC_ST_CTRL_R = 0x5;

    //////////////////////
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0xFF;
    GPIO_PORTF_DEN_R |= (sw1 | sw2);
    GPIO_PORTF_DIR_R &= ~(sw1 | sw2);
    GPIO_PORTF_PUR_R |= (sw1 | sw2);

    ///////////////////////
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
    float counterValue = 0;
    char counterStr[] = "";
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
                char c = key_pressed + 48;
                strncat(counterStr, &c, 1);
                strLen++;

                counterValue = atof(counterStr);
                write_lcd(counterStr, strLen);
                write_lcd(".00", 3);
            }
        }

        // excution loop
        if (strLen == 4 || sw1_on) //if 4 digit number or left switch is pressed
        {
            while (sw2_off && counterValue) // rigth key not pressed or counter value != 0
            {
                lcd_command(0x80);

                counterValue = counterValue - 0.01;
                ftoa(counterValue, counterStr, 2);
                write_lcd(counterStr, strlen(counterStr));

                while (!(NVIC_ST_CTRL_R & 0x10000))
                {
                }

                sw2_off = (GPIO_PORTF_DATA_R & sw2);

            }
            if (!counterValue)
            {
                write_lcd("countdown end", 13);
                blink(50);
                lcd_command(0x01);
            }
        }
    }
    return 0;
}
