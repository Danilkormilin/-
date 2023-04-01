#include <math.h>
#include "graphics.h"
#include "task.h"

void fill_1(int top, int left, int width, int height)
{
   double x, y;
   int c;
   
   for (x = 0; x < width; x++)
   {
      for (y = 0; y < height; y++)
      {
         c = y / height * COLOR_MAX;
         putpixel(left + x, top + y, COLOR(0, c / 2, c));
      }
   }
}

void fill_2(int top, int left, int width, int height)
{
   
}

void fill_3(int top, int left, int width, int height)
{
   
}

void fill_4(int top, int left, int width, int height)
{
   
}

void fill_5(int top, int left, int width, int height)
{
   
}

void fill_6(int top, int left, int width, int height)
{
   
}

void fill_7(int top, int left, int width, int height)
{
   double x, y, d, r;
   int c;
   
   r = (width > height ? height : width) / 2;
   
   for (x = 0; x < width; x++)
   {
      for (y = 0; y < height; y++)
      {
         d = sqrt((x - width / 2) * (x - width / 2) +
                  (y - height / 2) * (y - height / 2)) / r;
         c = d < 1 ? (1 - d) * COLOR_MAX : 0;
         putpixel(left + x, top + y, COLOR(0, c / 2, c));
      }
   }
}

void fill_8(int top, int left, int width, int height)
{
   
}

void save()
{
   int width, height;
   IMAGE *output;
   
   width  = getmaxx() + 1;
   height = getmaxy() + 1;
   output = createimage(width, height);
   
   getimage(0, 0, width - 1, height - 1, output);
   saveBMP("output.bmp", output);
   freeimage(output);
}