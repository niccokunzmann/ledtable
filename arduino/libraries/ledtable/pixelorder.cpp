#include "ledtable.h"

void id(LEDTable* ledtable, int*x, int*y) 
{
}

void snake(LEDTable* ledtable, int*x, int*y) 
{
  if ((*y) & 1) {
    *x = ledtable->width() - *x - 1;
  }
}

void mirror_horizontally(LEDTable* ledtable, int*x, int*y) 
{
  *y = ledtable->height() - 1 - *y;
}

void mirror_vertically(LEDTable* ledtable, int*x, int*y) 
{
  *x = ledtable->height() - 1 - *x;
}

void rotate_90(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->height() - *y; // todo: is that correct?
  *y = *x;
}

void rotate_180(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->width() - *x - 1;
  *y = ledtable->height() - *y - 1;
}

void rotate_270(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = *y;
  *y = ledtable->width() - *x; // todo: is that correct?
}

void flip_xy(LEDTable* ledtable, int*x, int*y)
{
  int t = *x;
  *x = *y;
  *y = t;
}

