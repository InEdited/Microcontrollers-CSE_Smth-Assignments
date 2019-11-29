#include "stdio.h"
#include "tm4c123gh6pm.h"
#include "7seg.h"
#include "pad.h"

void delay(int counter)
{
  int c =0;
  while(c < counter)
  {
    c++;
  }
}

 void main()
{
  
  
  
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R1;            
  
  //////////////////////////////////////////////////////////////////////////////
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R2; //PORT C
  SYSCTL_RCGCGPIO_R |=  SYSCTL_RCGCGPIO_R4; //PORT E
  
  GPIO_PORTC_DEN_R |=  columns;
  GPIO_PORTC_DIR_R &= ~columns;
  GPIO_PORTC_PUR_R |=  columns;
  
  GPIO_PORTE_ODR_R |=  rows;
  GPIO_PORTE_DEN_R |=  rows;
  GPIO_PORTE_DIR_R |=  rows;
  //////////////////////////////////////////////////////////////////////////////
  GPIO_PORTB_DEN_R  |=  0xFF;                          
  GPIO_PORTB_DIR_R  |=  0xFF;                          

  struct Seg7 disp = init_seg7(&GPIO_PORTB_DATA_R, 1);

  unsigned int key_pressed = 0;
  
  while(1)
  { 
    if (poll_keypad())
    {
      key_pressed = get_key();
    }

    set_val_seg7(&disp, key_pressed);
    draw_seg7(&disp);
  }
}
  

