#ifndef CONTROL_H
#define CONTROL_H

enum control_values { NONE = -1, EXIT, SAVE,
                      FILL_1, FILL_2, FILL_3, FILL_4,
                      FILL_5, FILL_6, FILL_7, FILL_8,
                      N_CONTROLS };

struct Control
{
   int left;
   int top;
   int right;
   int bottom;
};

void create_control(int, int, int, const char*);
int select_control();

#endif