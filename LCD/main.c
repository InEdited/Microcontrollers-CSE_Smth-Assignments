#include "tm4c123gh6pm.h"

#define switch2 0x01
#define switch1 0x10

void delayUs(int n){
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<3;j++){}
}

void delayMilli(int n){
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<3180;j++){}

}

void init(void){
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
        SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R1;
        SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R0; //PORT A
        GPIO_PORTF_LOCK_R = 0x4C4F434B ;
        GPIO_PORTF_CR_R  =0x01F;

        GPIO_PORTF_DIR_R &= ~(switch1|switch2);
        //GPIO_PORTF_PUR_R |= (switch1);
        //GPIO_PORTF_PUR_R |= (switch2);
        //GPIO_PORTF_DEN_R |= (WHITE | switch1 | switch2);
        GPIO_PORTF_PUR_R |= 0x11;
        GPIO_PORTF_DEN_R |= 0x1F;

        //////////////////////////////////////////////////////////////////////////////
        GPIO_PORTA_DEN_R  |=  0x1C;
        GPIO_PORTA_DIR_R  |=  0x1C;
        GPIO_PORTB_DEN_R  |=  0xFF;
        GPIO_PORTB_DIR_R  |=  0xFF;

}

void lcd_data(unsigned char data){
    GPIO_PORTA_DATA_R = 0x4;
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R = 0x14;
    delayUs(1);
    GPIO_PORTA_DATA_R = 0x00;
    delayUs(40);
}

void lcd_command(unsigned char command){
    GPIO_PORTA_DATA_R = 0x00;
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R |= 0x10;
    delayUs(1);
    GPIO_PORTA_DATA_R = 0x00;
    if(command < 4){
        delayMilli(2);
    }else{
        delayUs(40);
    }
}
void write_lcd(unsigned char* data, short length){
    int i;
    for(i = 0; i<length;i++){
        lcd_data(data[i]);
        delayMilli(1);
    }
}
/**
 * main.c
 */
int main(void)
{
    init();
    unsigned char* bois[3] = {"3ILA2","RUBY","ASSEM"};
    int nameSize[3] = {5,4,5};
    int current_name = 0;

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
    //lcd_data('A');
    //write_lcd("LOL", 3);
//
//
    write_lcd("LEL", 3);
    while(1){
            if(!(GPIO_PORTF_DATA_R & switch1)){
                if(current_name < 2){
                    current_name++;
                    }
                else{
                    current_name=0;
                }

                lcd_command(0x01);
                write_lcd(bois[current_name], nameSize[current_name]);
                //write_lcd("LEL", 3);
                delayUs(50000);
            }
            if(!(GPIO_PORTF_DATA_R & switch2)){
                if(current_name > 0){
                    current_name--;
                }
                else{
                    current_name=2;
                }

                lcd_command(0x01);
                write_lcd(bois[current_name], nameSize[current_name]);
                delayUs(50000);
            }
        }
	return 0;
}
