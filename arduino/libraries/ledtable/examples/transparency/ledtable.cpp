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

void LEDTable::updateColor(uint16_t index, Color color)
{
  if (color == color_transparent) return;
  if (!(color & 0xff000000)) 
  {
    strip.setPixelColor(index, color);
    return;
  }
  // mixing different transparencies
  uint16_t transparency = ALPHA(color);
  Color color0 = strip.getPixelColor(index);
  uint16_t red = (RED(color) * (256 - transparency) + RED(color0) * transparency) >> 8;
  uint16_t green = (GREEN(color) * (256 - transparency) + GREEN(color0) * transparency) >> 8;
  uint16_t blue = (BLUE(color) * (256 - transparency) + BLUE(color0) * transparency) >> 8;
  strip.setPixelColor(index, RGB(red, green, blue));
}

void LEDTable::fill(Color color) 
{
  for (int i = width() * height() - 1; i >= 0; i--) {
    updateColor(i, color);
  }
}

void LEDTable::fill(int x, int y, Color color) 
{
  this->pixelorder(this, &x, &y);
  if ( x < 0 || x >= width() || y < 0 || y >= height()) return;
  updateColor(x + y * width(), color);
}

void LEDTable::fill(int x1, int y1, int x2, int y2, Color color) 
{
  int x_min = min(x1, x2);
  int x_max = max(x1, x2);
  int y_min = min(y1, y2);
  int y_max = max(y1, y2);
  for (int x = x_min; x < x_max; x++) 
  {
    for (int y = y_min; y < y_max; y++)
    {
      fill(x, y, color);
    }
  }
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
