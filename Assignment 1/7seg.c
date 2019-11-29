#include "7seg.h"

const int SEG7_ARR[] = {
  SEG7_0,SEG7_1,SEG7_2,SEG7_3,
  SEG7_4,SEG7_5,SEG7_6,SEG7_7,
  SEG7_8,SEG7_9,SEG7_A,SEG7_B,
  SEG7_C,SEG7_D,SEG7_E,SEG7_F};

struct Seg7 init_seg7(unsigned long volatile *port_register, short common_anode)
{
  struct Seg7 disp;
  disp.port_register = port_register;
  disp.common_anode = common_anode;
  disp.current_val = 0;
  disp.dp = 0;
  
  return disp;
}

void clear_seg7(struct Seg7 *disp)
{
  if(disp->common_anode)
    *(disp->port_register) |= ~0x0;
  else
    *(disp->port_register) &= 0x0;
}

void draw_seg7(struct Seg7 *disp) {
  clear_seg7(disp);
  
  
  long value = SEG7_ARR[disp->current_val];
  if (disp->dp)
    value |= SEG7_DP;
  
  if(disp->common_anode)
    *(disp->port_register) &= ~value;
  else
    *(disp->port_register) |= value;
  
}

void incr_seg7(struct Seg7 *disp)
{
  disp->current_val++;
  if(disp->current_val>15)
    disp->current_val = 0;
}

void decr_seg7(struct Seg7 *disp)
{
  disp->current_val--;
  if(disp->current_val < 0)
    disp->current_val = 15;
}

void set_val_seg7(struct Seg7 *disp, short val)
{
  if(val > 15)
    disp->current_val = 15;
  else if (val < 0)
    disp->current_val = 0;
  else 
    disp->current_val = val;
}

void reset_seg7(struct Seg7 *disp)
{
  set_val_seg7(disp, 0);
}
  
void showdp_seg7(struct Seg7 *disp, short flag)
{
  disp->dp = flag;
}
