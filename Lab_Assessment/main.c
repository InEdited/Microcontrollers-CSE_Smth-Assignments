#include "tm4c123gh6pm.h"
#include <stdint.h>

#define DARK 0x0
#define RED 0x2
#define BLUE 0x4
#define GREEN 0x8
#define YELLOW 0xA
#define CYAN 0xC
#define PINK 0x7
#define WHITE 0xE

#define switch2 0x01
#define switch1 0x10

void port_init(void){
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
        GPIO_PORTF_LOCK_R = 0x4C4F434B ;
        GPIO_PORTF_CR_R  =0x01F;
        GPIO_PORTF_DIR_R |= (WHITE);
        GPIO_PORTF_DIR_R &= ~(switch1|switch2);
        //GPIO_PORTF_PUR_R |= (switch1);
        //GPIO_PORTF_PUR_R |= (switch2);
        //GPIO_PORTF_DEN_R |= (WHITE | switch1 | switch2);
        GPIO_PORTF_PUR_R |= 0x11;
        GPIO_PORTF_DEN_R |= 0x1F;

}

void set_color(uint8_t color){

    GPIO_PORTF_DATA_R = 0x11;
    GPIO_PORTF_DATA_R |= (color);
    //GPIO_PORTF_DATA_R &= ~(color);
}

void delay(int delayNum){
    int counter =0;
    while(counter<delayNum){
        counter++;
    }
}

int main(void)
{
    uint8_t colors[8] = {DARK,RED,BLUE,GREEN,YELLOW,CYAN,PINK,WHITE};
    uint8_t current_color =0    ;

    port_init();
    //GPIO_PORTF_DATA_R  |= CYAN;
    set_color(DARK);

    while(1){
        if(!(GPIO_PORTF_DATA_R & switch1)){
            if(current_color < 7){
                current_color++;
                }
            else{
                current_color=0;
            }
            set_color(colors[current_color]);
            delay(500000);
        }
        if(!(GPIO_PORTF_DATA_R &switch2)){
            if(current_color>1){
                current_color--;
            }else{
                current_color=7;
            }
            set_color(colors[current_color]);
            delay(500000);
        }
    }



	return 0;
}
