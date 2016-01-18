
#include "ledtable.h"

void pixelorder::id(LEDTable* ledtable, int*x, int*y) 
{
}

void pixelorder::snake(LEDTable* ledtable, int*x, int*y) 
{
  if ((*y) & 1) {
    *x = ledtable->width - *x - 1;
  }
}

void pixelorder::mirror_horizontally(LEDTable* ledtable, int*x, int*y) 
{
  *y = ledtable->height - 1 - *y;
}

void pixelorder::mirror_vertically(LEDTable* ledtable, int*x, int*y) 
{
  *x = ledtable->height - 1 - *x;
}

void pixelorder::rotate_90(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->height - *y; // todo: is that correct?
  *y = *x;
}

void pixelorder::rotate_180(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->width - *x;
  *y = ledtable->height - *y;
}

void pixelorder::rotate_270(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = *y;
  *y = ledtable->width - *x; // todo: is that correct?
}



LEDTable::LEDTable(int pin,
    int width,
    int height,
    PixelOrder pixelorder,
    neoPixelType t) : width(width), height(height), pixelorder(pixelorder) 
{
  strip = Adafruit_NeoPixel(width * height, pin, t);
}

void LEDTable::begin(){
  strip.begin();
} 

void LEDTable::show(){
  strip.show();
} 


void LEDTable::fill(Color color) 
{
  for (int i = width * height - 1; i >= 0; i--) {
    strip.setPixelColor(i, color);
  }
}

void LEDTable::fill(int x, int y, Color color) 
{
  this->pixelorder(this, &x, &y);
  strip.setPixelColor(x + y * width, color);
  if ( x < 0 || x >= width || y < 0 || y >= height) return;
}

