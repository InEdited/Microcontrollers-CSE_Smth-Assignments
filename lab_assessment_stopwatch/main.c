#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <stdbool.h>

#define switch2 0x01
#define switch1 0x10

void port_init(void){
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
        GPIO_PORTF_LOCK_R = 0x4C4F434B ;
        GPIO_PORTF_CR_R  =0x01F;
        GPIO_PORTF_DIR_R &= ~(switch1|switch2);
        GPIO_PORTF_PUR_R |= 0x11;
        GPIO_PORTF_DEN_R |= 0x1F;

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

const int CLOCKS_PER_SECOND = 15999999;

struct Watch {
    int hours;
    int mins;
    int secs;
};

void print_watch(struct Watch* watch, unsigned char* string) {
    //sprintf(string, "%d %d %d", watch->hours, watch->mins, watch->secs);
    string[0] = watch->hours / 10 + '0';
    string[1] = watch->hours % 10+'0';
    string[2] = ':';
    string[3] = watch->mins / 10 +'0';
    string[4] = watch->mins % 10+'0';
    string[5] = ':';
    string[6] = watch->secs / 10 +'0';
    string[7] = watch->secs+'0';

}

void inc_watch(unsigned char* string) {
    *(string+7)+=1;
    if(string[7] > '9'){
        string[7] = '0';
        string[6]++;
        if(string[6] == '6'){
            string[6] = '0';
            string[4]++;
        }
    }
}

void timer_init(int time_ms) {
    SYSCTL_RCGCTIMER_R |= 0x01;
    TIMER0_CTL_R = 0x0;
    TIMER0_CFG_R = 0x0;

    TIMER0_TAMR_R = 0x01;
    TIMER0_TAILR_R = 16000 * time_ms -1;

    TIMER0_ICR_R = 0x1;

    TIMER0_CTL_R = 0x1;
    while((TIMER0_RIS_R & 0x1) == 0);
    TIMER0_ICR_R = 0x1;
}

void wait_for_timer() {
    while((TIMER0_RIS_R & 0x1) == 0);
    TIMER0_ICR_R = 0x1;
}

void init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; //PORT A
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //PORT B

    GPIO_PORTA_DEN_R |= 0x1C;
    GPIO_PORTA_DIR_R |= 0x1C;

    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DIR_R |= 0xFF;


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
    port_init();
    init_LCD();
    bool  flag = false;
    struct Watch stop_watch = {.hours = 0, .mins = 0, .secs = 0};
    unsigned char time[8] = "00:00:00";

    write_lcd("start", 5);
    while (1)
    {
        if(!(GPIO_PORTF_DATA_R & switch2)){
            flag = true;
        }
        if(!(GPIO_PORTF_DATA_R & switch1)){
            flag = false;
        }
        //  qprint_watch(&stop_watch, time);
        while(flag){
            if(!(GPIO_PORTF_DATA_R & switch2)){
                flag = true;
            }
            if(!(GPIO_PORTF_DATA_R & switch1)){
                flag = false;
            }


            write_lcd(time, 8);
            timer_init(1000);

            //wait_for_timer();

            inc_watch(time);
            lcd_command(0x01);
        }
    }
//        unsigned char a = '9';
//        while (1)
//        {
//            lcd_data(a);
//
//            while (!(NVIC_ST_CTRL_R & 0x10000))
//            {
//            }
//
//            lcd_command(0x01);
//
//            a--;
//
//            if (a == '0' - 1)
//            {
//                a = '9';
//            }
//        }
    return 0;
}
