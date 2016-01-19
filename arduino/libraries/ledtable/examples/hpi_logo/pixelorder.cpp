#include "ledtable.h"

void pixelorder::id(LEDTable* ledtable, int*x, int*y) 
{
}

void pixelorder::snake(LEDTable* ledtable, int*x, int*y) 
{
  if ((*y) & 1) {
    *x = ledtable->width() - *x - 1;
  }
}

void pixelorder::mirror_horizontally(LEDTable* ledtable, int*x, int*y) 
{
  *y = ledtable->height() - 1 - *y;
}

void pixelorder::mirror_vertically(LEDTable* ledtable, int*x, int*y) 
{
  *x = ledtable->height() - 1 - *x;
}

void pixelorder::rotate_90(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->height() - *y; // todo: is that correct?
  *y = *x;
}

void pixelorder::rotate_180(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->width() - *x;
  *y = ledtable->height() - *y;
}

void pixelorder::rotate_270(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = *y;
  *y = ledtable->width() - *x; // todo: is that correct?
}


