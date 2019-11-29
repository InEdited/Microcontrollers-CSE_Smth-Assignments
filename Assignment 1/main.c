#include "stdio.h"
#include "tm4c123gh6pm.h"
#include "7seg.h"

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
    sw1_on = !(GPIO_PORTF_DATA_R & sw1);
    sw2_on = !(GPIO_PORTF_DATA_R & sw2);
    
    if(sw1_on && sw2_on)
    {
      reset_seg7(&disp);
      delay(700000);
    }
    else if ( sw1_on )
    {
      decr_seg7(&disp);
      delay(700000);
    }
    else if ( sw2_on )
    {
      incr_seg7(&disp);
      delay(700000);
    }
    draw_seg7(&disp);
  }
  
  
  return 0;
}
