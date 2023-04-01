#include "graphics.h"
#include "control.h"
#include "task.h"

int main()
{
   initwindow(600, 450);
   
   create_control(FILL_1,   0, 400, "fill_1.bmp");
   create_control(FILL_2,  50, 400, "fill_2.bmp");
   create_control(FILL_3, 100, 400, "fill_3.bmp");
   create_control(FILL_4, 150, 400, "fill_4.bmp");
   create_control(FILL_5, 200, 400, "fill_5.bmp");
   create_control(FILL_6, 250, 400, "fill_6.bmp");
   create_control(FILL_7, 300, 400, "fill_7.bmp");
   create_control(FILL_8, 350, 400, "fill_8.bmp");
   create_control(SAVE,   400, 400, "save.bmp");
   create_control(EXIT,   500, 400, "exit.bmp");
   
   int left = 0, top = 0, width = 600, height = 400;
   
   while (true)
   {
      while (mousebuttons() != 1);
      switch (select_control())
      {
         case NONE: break;
         case FILL_1: fill_1(left, top, width, height); break;
         case FILL_2: fill_2(left, top, width, height); break;
         case FILL_3: fill_3(left, top, width, height); break;
         case FILL_4: fill_4(left, top, width, height); break;
         case FILL_5: fill_5(left, top, width, height); break;
         case FILL_6: fill_6(left, top, width, height); break;
         case FILL_7: fill_7(left, top, width, height); break;
         case FILL_8: fill_8(left, top, width, height); break;
         case SAVE: save(); break;
         case EXIT: closegraph(); return 0;
      }
   }
}