#include "stdio.h"
#include "tm4c123gh6pm.h"
#include "7seg.h"
#include "pad.h"

#define sw1 0x1 << 4
#define sw2 0x1 << 0

void delay(int counter)
{
  int c =0;
  while(c < counter)
  {
    c++;
  }
}

int main()
{
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R1;            
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R5;
  
  //////////////////////////////////////////////////////////////////////////////
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R2; //port c
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R4; //port e
  
  GPIO_PORTC_DEN_R |=  (column1 | column2 | column3 | column4);
  GPIO_PORTC_DIR_R &= ~(column1 | column2 | column3 | column4);
  GPIO_PORTC_PUR_R |=  (column1 | column2 | column3 | column4);
  
  GPIO_PORTE_ODR_R |=  (row1 | row2| row3 | row4);
  GPIO_PORTE_DEN_R |=  (row1 | row2| row3 | row4);
  GPIO_PORTE_DIR_R |=  (row1 | row2| row3 | row4);
  //////////////////////////////////////////////////////////////////////////////
  
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0xFF;
  GPIO_PORTF_DEN_R |=  (sw1 | sw2);
  GPIO_PORTF_DIR_R &= ~(sw1 | sw2);
  GPIO_PORTF_PUR_R |=  (sw1 | sw2);
  
  GPIO_PORTB_DEN_R  |=  0xFF;                          
  GPIO_PORTB_DIR_R  |=  0xFF;                          

  struct Seg7 disp = init_seg7(&GPIO_PORTB_DATA_R, 1);
  
  int sw1_on, sw2_on;
  
  while(1)
  {
    int k = keyPressed();
    set_val_seg7(&disp,k);
    draw_seg7(&disp);
  }
    return 0;
}
  

