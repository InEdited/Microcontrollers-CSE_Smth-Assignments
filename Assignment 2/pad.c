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
    GPIO_PORTE_DATA_R &= ~(1 << 4-i);
    col = get_column_value((GPIO_PORTC_DATA_R & columns) >> 4);
    
    if (col) {
      break;
    }
  }
  
  return get_button(row, col);
}

int get_column_value(int val) {
  switch (val) {
  case 14:
    return 1;
  case 13:
    return 2;
  case 11:
    return 3;
  case 7:
    return 4;
  }
  return 0;
}

int get_button (int row, int column) {
  if ( row && column )
    return KEYMAP[row - 1][column - 1];
  return -1;
}
