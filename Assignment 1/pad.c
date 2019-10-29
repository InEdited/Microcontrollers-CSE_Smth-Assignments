#include "tm4c123gh6pm.h"
#include "7seg.h"
#include "pad.h"

const unsigned int keymap[4][4] = {
{ 1, 2, 3, 10},

{ 4, 5, 6, 11},

{ 7, 8, 9, 12},

{ 14, 0, 15, 13},

};

unsigned int keyPressed()
{
  GPIO_PORTE_DATA_R &= ~(row1 | row2| row3 | row4);
  char d = ~(GPIO_PORTE_DATA_R | (0xf));
  if(d)
  {
    return keyIdentification();
  }
  return 0;
}






unsigned int keyIdentification()
{
  int c = 1;
  int s = 0;
  while(c < 5)
  {
    char ss = ~(1 << c);
    c++;
    GPIO_PORTE_DATA_R &= ~ss;
    
    if ((int)GPIO_PORTC_DATA_R == 237)
    {
      s= 0 ;
      break;
    }
    else if((int)GPIO_PORTC_DATA_R == 221)
    {
      s = 1;
      break;
    }
    else if((int)GPIO_PORTC_DATA_R == 189)
    {
      s = 2 ;
      break;
    }
    else if ((int)GPIO_PORTC_DATA_R == 125)
    {
      s = 3;
      break;
    }
  }
   return keymap[c][s];
}