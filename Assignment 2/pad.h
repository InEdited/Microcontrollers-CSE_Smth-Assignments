#ifndef KEYPAD_HEADER
#define KEYPAD_HEADER

#define KEYPAD_COLUMN_1 0x1 << 4        // PORT C
#define KEYPAD_COLUMN_2 0x1 << 5        // PORT C
#define KEYPAD_COLUMN_3 0x1 << 6        // PORT C
#define KEYPAD_COLUMN_4 0x1 << 7        // PORT C

#define KEYPAD_ROW_1 0x1 << 1           // PORT E
#define KEYPAD_ROW_2 0x1 << 2           // PORT E
#define KEYPAD_ROW_3 0x1 << 3           // PORT E
#define KEYPAD_ROW_4 0x1 << 4           // PORT E

#define columns (KEYPAD_COLUMN_1        | KEYPAD_COLUMN_2       | KEYPAD_COLUMN_3       | KEYPAD_COLUMN_4)
#define rows    (KEYPAD_ROW_1           | KEYPAD_ROW_2          | KEYPAD_ROW_3          | KEYPAD_ROW_4)

int poll_keypad();
int get_key();
int get_column_value(int val);
int get_button(int row, int column);

#endif