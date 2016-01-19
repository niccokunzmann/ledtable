#include "ledtable.h"

LEDTable::LEDTable(int pin,
    int width,
    int height,
    PixelOrder pixelorder,
    neoPixelType t) : _width(width), _height(height), pixelorder(pixelorder) 
{
  strip = Adafruit_NeoPixel(this->width() * this->height(), pin, t);
}

void LEDTable::begin(){
  strip.begin();
} 

void LEDTable::show(){
  strip.show();
} 


void LEDTable::fill(Color color) 
{
  for (int i = width() * height() - 1; i >= 0; i--) {
    strip.setPixelColor(i, color);
  }
}

void LEDTable::fill(int x, int y, Color color) 
{
  this->pixelorder(this, &x, &y);
  if ( x < 0 || x >= width() || y < 0 || y >= height()) return;
  strip.setPixelColor(x + y * width(), color);
}

void LEDTable::print(Text* text, int x, int y, Color text_color, Color background_color) {
  Serial.print("x2: "); Serial.println(x);
  text->printOn(this, x, y, text_color, background_color);
}

const int LEDTable::height()
{
  return _height;
}

const int LEDTable::width()
{
  return _width;
}
