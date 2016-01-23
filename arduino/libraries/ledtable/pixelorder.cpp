#include "ledtable.h"

/*
 * We request that the point x=0, y=0 are inside.
 * Otherwise, height and width will not work.
 * 
 * use originalWidth() and originalHeight()
 */


void id(LEDTable* ledtable, int*x, int*y) 
{
}

void snake(LEDTable* ledtable, int*x, int*y) 
{
  if ((*y) & 1) {
    *x = ledtable->originalWidth() - *x - 1;
  }
}

void mirror_horizontally(LEDTable* ledtable, int*x, int*y) 
{
  *y = ledtable->originalHeight() - 1 - *y;
}

void mirror_vertically(LEDTable* ledtable, int*x, int*y) 
{
  *x = ledtable->originalHeight() - 1 - *x;
}

void rotate_90(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->originalHeight() - *y; // todo: is that correct?
  *y = *x;
}

void rotate_180(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->originalWidth() - *x - 1;
  *y = ledtable->originalHeight() - *y - 1;
}

void rotate_270(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = *y;
  *y = ledtable->originalWidth() - *x; // todo: is that correct?
}

void flip_xy(LEDTable* ledtable, int*x, int*y)
{
  int t = *x;
  *x = *y;
  *y = t;
}

