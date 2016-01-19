
#ifndef LEDTABLE_H
#define LEDTABLE_H

#include <Adafruit_NeoPixel.h>
#include "html-color-names.h"
#include "Arduino.h"

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

#define DEFINE_PIXELORDER(name, o1, o2, o3, o4, o5) function name(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); o3( ledtable, x, y); o4( ledtable, x, y); o5( ledtable, x, y); };
#define DEFINE_PIXELORDER(name, o1, o2, o3, o4) function name(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); o3( ledtable, x, y); o4( ledtable, x, y); };
#define DEFINE_PIXELORDER(name, o1, o2, o3) function name(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); o3( ledtable, x, y); };
#define DEFINE_PIXELORDER(name, o1, o2) function name(LEDTable* ledtable, int*x, int*y) { o1( ledtable, x, y); o2( ledtable, x, y); };

#define PIXELORDER(order) (&(pixelorder::order))

class Text;

class LEDTable 
{
private:
  int _height;
  int _width;
  PixelOrder pixelorder;
  Adafruit_NeoPixel strip;

public:
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
  
  void print(Text* text, int x = 0, int y = 0, Color text_color = color_default, Color background_color = color_default);
  
  const int width();
  const int height();
};

class Text
{
private:
  const char* _text;
  int _width;
  int _height;
  Color _text_color;
  Color _background_color;
public:
  Text(const char* text, Color text_color = color_white, Color background_color = color_black);
  
  void printOn(LEDTable* ledtable, int x = 0, int y = 0, Color text_color = color_default, Color background_color = color_default);
  int width();
  const int width(char character);
  int height();
  const int height(char character);
  const char* text();
  
  Color background_color(); // get the background color
  void background_color(Color background_color);
  Color text_color();
  void text_color(Color text_color);
};

                          //x1111122222333334444455555
#define UNKNOWN_CHARACTER 0b11111110001101011000111111
#define CHARACTER_HEIGHT 5
#define LETTERS 127
extern uint32_t characterToPixels[LETTERS];
#define getPixels(character) (character < LETTERS ? characterToPixels[character] : UNKNOWN_CHARACTER)

#endif // LEDTABLE_H
