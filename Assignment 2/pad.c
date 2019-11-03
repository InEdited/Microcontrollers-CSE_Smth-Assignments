#include "tm4c123gh6pm.h"
#include "7seg.h"
#include "pad.h"

const unsigned int KEYMAP[4][4] = 
{
            {   1,      2,      3,      10},
              
            {   4,      5,      6,      11},
              
            {   7,      8,      9,      12},
              
            {  14,      0,     15,      13},

};

int poll_keypad() {
  GPIO_PORTE_DATA_R &= ~rows;
  if ((GPIO_PORTC_DATA_R & columns) == 0xF0) 
    return 0;
  return 1;
}

int get_key() {
  short i = 4;
  int col = 0, 
      row = 0;
  
  while ( i --> 0 ) {
    row = 4-i;
    GPIO_PORTE_DATA_R |= rows;
    GPIO_PORTE_DATA_R &= ~(1 << i+1);
    //printf("Row_value: %d\n", 2<<i);
    col = get_column_value((GPIO_PORTC_DATA_R & columns) >> 4);
    
    if (col) {
      break;
    }
  }
  
  //printf("Col: %d, Row: %d\n", col, row);
  return get_button(row, col);
}

int get_column_value(int val) {
  //printf("val in `get_column_value()`: %d\n", val);
  switch (val) {
  case 14:
    return 4;
  case 13:
    return 3;
  case 11:
    return 2;
  case 7:
    return 1;
  }
  return 0;
}

int get_button (int row, int column) {
  if ( row && column )
    return KEYMAP[row - 1][column - 1];
  return -1;
}





//unsigned int keyIdentification()
//{
//  int c = 1;
//  int s = 0;
//  while(c < 5)
//  {
//    char ss = ~(1 << c);
//    c++;
//    GPIO_PORTE_DATA_R &= ~ss;
//    
//    if ((int)GPIO_PORTC_DATA_R == 237)
//    {
//      s= 0 ;
//      break;
//    }
//    else if((int)GPIO_PORTC_DATA_R == 221)
//    {
//      s = 1;
//      break;
//    }
//    else if((int)GPIO_PORTC_DATA_R == 189)
//    {
//      s = 2 ;
//      break;
//    }
//    else if ((int)GPIO_PORTC_DATA_R == 125)
//    {
//      s = 3;
//      break;
//    }
//  }
//   return keymap[c][s];
//}