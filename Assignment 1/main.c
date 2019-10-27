#include "stdio.h"
#include "tm4c123gh6pm.h"
#include "7seg.h"

#define sw1 0x1 << 4
#define sw2 0x1 << 0

int main()
{
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R1;            
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R5;            
  
  GPIO_PORTF_DEN_R |=  (sw1 | sw2);
  GPIO_PORTF_DIR_R &= ~(sw1 | sw2);
  GPIO_PORTF_PDR_R |=  (sw1 | sw2);
  
  GPIO_PORTB_DEN_R  |=  ~0x0;                          
  GPIO_PORTB_DIR_R  |=  ~0x0;                          

  struct Seg7 disp = init_seg7(&GPIO_PORTB_DATA_R, 0);
  
  int sw1_on, sw2_on;
  
  while(1)
  {
    sw1_on = GPIO_PORTF_DATA_R & sw1;
    sw2_on = GPIO_PORTF_DATA_R & sw2;
    
    if(sw1_on && sw2_on)
      reset_seg7(&disp);
    else if ( sw1_on )
      decr_seg7(&disp);
    else if ( sw2_on )
      incr_seg7(&disp);
    
    draw_seg7(&disp);
  }
  
  
  return 0;
}
