#ifndef SEG_7_HEADER
#define SEG_7_HEADER
//*****************************************************************************
//
// Port Register to 7_Segment Display
//                                        **************
//                                        * 0xPGFEDCBA *
//*****************************************************************************
#define SEG7_0  0x3f                    //* 0b00111111 *
#define SEG7_1  0x06                    //* 0b00000110 *
#define SEG7_2  0x5b                    //* 0b01011011 *
#define SEG7_3  0x4f                    //* 0b01001111 *
#define SEG7_4  0x66                    //* 0b01100110 *
#define SEG7_5  0x6d                    //* 0b01101101 *
#define SEG7_6  0x7d                    //* 0b01111101 *
#define SEG7_7  0x07                    //* 0b00000111 *
#define SEG7_8  0x7f                    //* 0b01111111 *
#define SEG7_9  0x6f                    //* 0b01101111 *
#define SEG7_A  0x77                    //* 0b01110111 *
#define SEG7_B  0x7c                    //* 0b01111100 *
#define SEG7_C  0x39                    //* 0b00111001 *
#define SEG7_D  0x5e                    //* 0b01011110 *
#define SEG7_E  0x79                    //* 0b01111001 *
#define SEG7_F  0x71                    //* 0b01110001 *
#define SEG7_DP 0x80                    //* 0b10000000 *
//*****************************************************************************
//
// Struct that stores the information
// that the 7-segment display need 
// frequently to operate
//
//
// This is used to save the address in memory of 
// the port that is used to send signals to the 
// 7-segment display
//
// ***unsigned long volatile *port_register***          // This is used to save the address in memory of 
//                                                      // the port that is used to send signals to the 
//                                                      // 7-segment display
//
// ***short common_anode***                             // This is a flag to make the switch between
//                                                      // common-anode and common-cathode 7-segment
//                                                      // displays as easy as switching a flag
//
// ***short dp***                                       // Flag to light up the dp LED
//
// ***short current_val***                              // The current value displayed. Always within
//                                                      // the range 0..15, (0..F) in hex, inclusive.
//*****************************************************************************
struct Seg7
{
  unsigned long volatile *port_register;
  short common_anode;
  short dp;
  short current_val;
};
//*****************************************************************************
//
// struct Seg7 init_seg7(unsigned long volatile *port_register, short common_anode)
//
// * Return:
//      - struct Seg7                                     
// * Parameters: 
//      - unsigned long volatile *port_register           // Pointer to the data register in the assigned port
//      - short common_anode                              // Flag to whether the display is common-anode or common-cathode
// * Description:
//      Initializes a Seg7 struct to be ready for immediate usage
//*****************************************************************************
struct Seg7 init_seg7(unsigned long volatile *port_register, short common_anode);
//*****************************************************************************
//
// void draw_seg7(struct Seg7 *disp)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
// * Description:
//      Uses the Seg7's inner register pointer to change the output to
//      its current value; using the preset constants defined above.
//*****************************************************************************
void draw_seg7(struct Seg7 *disp);
//*****************************************************************************
//
// void incr_seg7(struct Seg7 *disp)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
// * Description:
//      Increments the value that the 7-segment displays.
//      
//      ***Special Case: If the value exceeds 15, which is the largest value for a
//      hexadecimal digit, it overflows to 0.
//*****************************************************************************
void incr_seg7(struct Seg7 *disp);
//*****************************************************************************
//
// void decr_seg7(struct Seg7 *disp)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
// * Description:
//      Decrements the value that the 7-segment displays.
//      
//      ***Special Case: If the value goes lower than 0, which is the smallest value for a
//      hexadecimal digit, it loops back to 15, which is the largest.
//*****************************************************************************
void decr_seg7(struct Seg7 *disp);
//*****************************************************************************
//
// void set_val_seg7(struct Seg7 *disp, short val)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
//      - short val                                       // Value to set the 7-segment display to show
// * Description:
//      Sets value for the display
//
//      *** Special case: If `val` is larger than 15, it will clamp to 15, going no higher than that.
//      *** Special case: If `val` is smaller than 0, it will clamp to 0, going no lower than that.
//*****************************************************************************
void set_val_seg7(struct Seg7 *disp, short val);
//*****************************************************************************
//
// void clear_seg7(struct Seg7 *disp)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
// * Description:
//      Turns of all the segments in the display
//*****************************************************************************
void clear_seg7(struct Seg7 *disp);
//*****************************************************************************
//
// void reset_seg7(struct Seg7 *disp)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
// * Description:
//      Resets the value of the 7-segment display to 0
//*****************************************************************************
void reset_seg7(struct Seg7 *disp);
//*****************************************************************************
//
// void showdp_seg7(struct Seg7 *disp)
//
// * Parameters:
//      - struct Seg7 *disp                               // Pointer to the 7-segment instance
//      - short flag                                      // Flag for the DP bit in the display
// * Description:
//      Sets the DP LED according to the passed flag
//*****************************************************************************
void showdp_seg7(struct Seg7 *disp, short flag);

#endif