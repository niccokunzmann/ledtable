
#ifndef LEDTABLE_H
#define LEDTABLE_H

#include <Adafruit_NeoPixel.h>
#include "html-color-names.h"

typedef uint32_t Color;

class LEDTable;

typedef void (*PixelOrder)(LEDTable*,int*,int*);

namespace pixelorder {
  // x=x y=y
  void id(LEDTable* ledtable, int*x, int*y);

  // even row : left to right
  // odd row: right to left
  void snake(LEDTable* ledtable, int*x, int*y);
  
  void mirror_horizontally(LEDTable* ledtable, int*x, int*y);
  void mirror_vertically(LEDTable* ledtable, int*x, int*y);
  
  void rotate_90(LEDTable* ledtable, int*x, int*y);
  void rotate_180(LEDTable* ledtable, int*x, int*y);
  void rotate_270(LEDTable* ledtable, int*x, int*y);
  
  // this allows pixel orders such as circles or if you glued something wrongly.
};

#define DEFINE_PIXELORDER(o1, o2, o3, o4, o5) function(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); o3( ledtable, x, y); o4( ledtable, x, y); o5( ledtable, x, y); };
#define DEFINE_PIXELORDER(o1, o2, o3, o4) function(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); o3( ledtable, x, y); o4( ledtable, x, y); };
#define DEFINE_PIXELORDER(o1, o2, o3) function(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); o3( ledtable, x, y); };
#define DEFINE_PIXELORDER(o1, o2) function(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); };

#define PIXELORDER(order) (&(pixelorder::order))

class LEDTable 
{
public:
  int height;
  int width;
  PixelOrder pixelorder;
  Adafruit_NeoPixel strip;

  LEDTable(
    int pin,
    int width,
    int height,
    // function pointers http://www.cprogramming.com/tutorial/function-pointers.html
    PixelOrder pixelorder = PIXELORDER(id),
    neoPixelType t = NEO_GRB + NEO_KHZ800);
  
  void begin();
  void show();
  
  void fill(Color color);
  void fill(int x, int y, Color color);
};




#endif // LEDTABLE_H
