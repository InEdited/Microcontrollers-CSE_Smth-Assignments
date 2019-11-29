#include "tm4c123gh6pm.h"

#define switch2 0x01
#define switch1 0x10

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
    NVIC_ST_RELOAD_R = 16000000 - 1;
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
int main(void)
{
    init();
    init_LCD();

    while (1)
    {
        unsigned char a = '9';
        while (1)
        {
            lcd_data(a);

            while (!(NVIC_ST_CTRL_R & 0x10000))
            {
            }

            lcd_command(0x01);

            a--;

            if (a == '0' - 1)
            {
                a = '9';
            }
        }
    }
    return 0;
}
