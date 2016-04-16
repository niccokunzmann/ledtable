
#ifndef LEDTABLE_H
#define LEDTABLE_H

#include <Adafruit_NeoPixel.h>
#include "html-color-names.h"
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

#ifdef HAVE_HWSERIAL0
#define USE_SERIAL_CONNECTION true
#define SERIAL_COMMAND_CHARACTER "!"
#endif

typedef uint32_t Color;

class LEDTable;

// function pointers http://www.cprogramming.com/tutorial/function-pointers.html
typedef void (*PixelOrder)(LEDTable*,int*,int*);

// pixel order
  // x=x y=y
  void id(LEDTable* ledtable, int*x, int*y);

  // even row : left to right
  // odd row: right to left
  void snake(LEDTable* ledtable, int*x, int*y);

  /*  0   3   4   7   ...  n-4 n-1 n+0 n+1
   *  1   2   5   6   ...  n-3 n-2 n+3 n+2
   * p+2 p+3 p+4 p+7  ___________  n+4 n+5
   * p+1 p+0 p+5 p+6  _________  | n+7 n+6
   * p-2 p-1  +--------        | |
   * p-3 p-4  | +------        | |
   *          | |              | |  .   .
   *  .   .   | |              | |  .   .
   *  .   .   | |              | |  .   .
   *  .   .   | |              | |
   *          | |              | |
   * o+6 o+7  | +--------------+ | m-4 m-3
   * o+5 o+4  +------------------+ m-1 m-2
   * o+2 o+3 o-2 o-3  ...  m+6 m+5 m+0 m+1
   * o+1 o+0 o-1 o-4  ...  m+7 m+4 m+3 m+2
   */
  void double_spiral(LEDTable* ledtable, int*x, int*y);
  
  void mirror_horizontally(LEDTable* ledtable, int*x, int*y);
  void mirror_vertically(LEDTable* ledtable, int*x, int*y);
  
  void rotate_90(LEDTable* ledtable, int*x, int*y);
  void rotate_180(LEDTable* ledtable, int*x, int*y);
  void rotate_270(LEDTable* ledtable, int*x, int*y);

  void flip_xy(LEDTable* ledtable, int*x, int*y);

template <PixelOrder o1> 
void PIXELORDER(LEDTable* ledtable, int*x, int*y) 
{
  o1(ledtable, x, y);
};
template <PixelOrder o1, PixelOrder o2> 
void PIXELORDER(LEDTable* ledtable, int*x, int*y) 
{
  o1(ledtable, x, y);
  o2(ledtable, x, y);
};
template <PixelOrder o1, PixelOrder o2, PixelOrder o3> 
void PIXELORDER(LEDTable* ledtable, int*x, int*y) 
{
  o1(ledtable, x, y);
  o2(ledtable, x, y);
  o3(ledtable, x, y);
};
template <PixelOrder o1, PixelOrder o2, PixelOrder o3, PixelOrder o4> 
void PIXELORDER(LEDTable* ledtable, int*x, int*y) 
{
  o1(ledtable, x, y);
  o2(ledtable, x, y);
  o3(ledtable, x, y);
  o4(ledtable, x, y);
};
template <PixelOrder o1, PixelOrder o2, PixelOrder o3, PixelOrder o4, PixelOrder o5> 
void PIXELORDER(LEDTable* ledtable, int*x, int*y) 
{
  o1(ledtable, x, y);
  o2(ledtable, x, y);
  o3(ledtable, x, y);
  o4(ledtable, x, y);
  o5(ledtable, x, y);
};

template <PixelOrder o1, PixelOrder o2, PixelOrder o3, PixelOrder o4, PixelOrder o5, PixelOrder o6> 
void PIXELORDER(LEDTable* ledtable, int*x, int*y) 
{
  o1(ledtable, x, y);
  o2(ledtable, x, y);
  o3(ledtable, x, y);
  o4(ledtable, x, y);
  o5(ledtable, x, y);
  o6(ledtable, x, y);
};

class Text;

class LEDTable 
{
private:
  int _height;
  int _width;
  PixelOrder _pixelorder;
  Adafruit_NeoPixel* strip;
  neoPixelType strip_type;
  uint8_t pin;
  bool _widthAndHeightAreSwitched;
#ifdef USE_SERIAL_CONNECTION
  bool* printed_pixels;
#endif

public:
  LEDTable(
    int pin,
    int width,
    int height,
    PixelOrder pixelorder = PIXELORDER<id>,
    neoPixelType t = NEO_GRB + NEO_KHZ800);
  
  ~LEDTable();
  
  void begin();
  void show();
  boolean canShow();
  
  void fill(Color color);
  // integer painting
  void fill(int x, int y, Color color);
  void fill(int x1, int y1, int x2, int y2, Color color);
  
  Color at(int x, int y);
  void ellipse(int x1, int y1, int x2, int y2, Color color);
  void line(int x1, int y1, int x2, int y2, Color color);
  void line(int x1, int y1, int x2, int y2, int x3, int y3, Color color);
  void line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Color color);
  void line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, Color color);
  void line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, Color color);
  void line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int x7, int y7, Color color);
  void line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int x7, int y7, int x8, int y8, Color color);
  void print(Text* text, int x = 0, int y = 0, Color text_color = color_default, Color backgroundColor = color_default);
    
  const int width();
  const int height();
  uint8_t brightness();
  void brightness(uint8_t brightness);
  uint8_t brightnessAt(int x, int y);

  const int originalWidth();
  const int originalHeight();
  
  const int minX();
  const int maxX();
  const int minY();
  const int maxY();
  const int middleX();
  const int middleY();
  
  bool isOutside(int x, int y);
  bool isInside(int x, int y);
  void pixelOrder(int* x, int* y);

#ifdef USE_SERIAL_CONNECTION
  void printToSerial(HardwareSerial* serial = &Serial);
  void printPixelOrderToSerial(HardwareSerial* serial = &Serial);
#endif
  
  // access the stripe or do not use the pixel order - no need to really use this
  int stripeIndex(int x, int y); // compute the pixel index on the stripe
  void updateColor(uint16_t stripIndex, Color color); // update the color at a given strip position
  int stripeIndexTransformed(int x, int y); // after pixelorder is applied, what is the stripe index
  void updateColorTransformed(int x, int y, Color color); // after pixelorder is applied, update the color on the stripe
  const bool isOutsideTransformed(const int x, const int y); // check if point (x, y) is outside
};

class Text
{
private:
  const char* _text;
  int _width;
  int _height;
  Color _color;
  Color _backgroundColor;
public:
  Text(const char* text, Color text_color = color_white, Color backgroundColor = color_black);
  
  void printOn(LEDTable* ledtable, int x = 0, int y = 0, Color text_color = color_default, Color backgroundColor = color_default);
  int width();
  const int width(char character);
  int height();
  const int height(char character);
  const char* text();
  
  Color backgroundColor(); // get the background color
  void backgroundColor(Color backgroundColor);
  Color color();
  void color(Color text_color);
};

                          //x1111122222333334444455555
#define UNKNOWN_CHARACTER 0b11111110001101011000111111
#define CHARACTER_HEIGHT 5
#define LETTERS 127
extern uint32_t characterToPixels[LETTERS];
#define getPixels(character) (character < LETTERS ? characterToPixels[character] : UNKNOWN_CHARACTER)

#define RGB(red, green, blue) Color((Color((red) & 0xff) << Color(16)) | (Color((green) & 0xff) << Color(8)) | Color((blue) & 0xff))
#define RGBA(red, green, blue, alpha) (RGB(red, green, blue) | (Color(alpha) << Color(24)))
#define ALPHA(rgb) (Color(rgb) >> 24)
#define transparent(rgb, alpha) (((Color(rgb) << 8) >> 8) | (Color(alpha) << 24)) 
#define RED(rgb) ((Color(rgb) >> 16) & 0xff)
#define GREEN(rgb) ((Color(rgb) >> 8) & 0xff)
#define BLUE(rgb) (Color(rgb) & 0xff)

Color random_color(uint8_t brightness = 0xff, uint8_t transparency = 0x00);
Color mixColors(Color color1, Color color2, uint8_t strength_of_color1 = 128);
const uint8_t brightness(const Color color);

#define STAMP_DEFAULT_COLOR color_white
#define STAMP_DEFAULT_BACKGROUND_COLOR color_transparent

class Stamp
{
private:
  uint32_t* _lines;
  uint8_t _width; 
  uint8_t _height;
  Color _color;
  Color _backgroundColor;
public:
  // create a Stamp from a array of lines. If width is greater than 32, it will be computed based on the given lines, because 32 is the largest width.
//  Stamp(const uint32_t* lines, uint8_t height, uint8_t width = 255, Color stamp_color = STAMP_DEFAULT_COLOR, Color backgroundColor = STAMP_DEFAULT_BACKGROUND_COLOR);
  Stamp(const uint32_t line1 = 0, const uint32_t line2 = 0, const uint32_t line3 = 0, const uint32_t line4 = 0, const uint32_t line5 = 0, const uint32_t line6 = 0, const uint32_t line7 = 0, const uint32_t line8 = 0);
  ~Stamp();
  
  void stamp(LEDTable* ledtable, int x, int y, Color color = color_default, Color backgroundColor = color_default);
  const bool canStamp();
  const uint8_t height();
  const uint8_t width();
  Color backgroundColor(); // get the background color
  void backgroundColor(Color backgroundColor);
  Color color();
  void color(Color stampColor);
  const uint32_t* lines();
  const uint32_t line(uint8_t index);
  void computeWidth();
  const bool isSet(int x, int y);
  const bool isFree(int x, int y);
};

#endif // LEDTABLE_H
