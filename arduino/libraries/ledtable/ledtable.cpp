#include "ledtable.h"

LEDTable::LEDTable(int pin,
    int width,
    int height,
    PixelOrder pixelorder,
    neoPixelType t) : _width(width), _height(height), pixelorder(pixelorder) 
{
  strip = Adafruit_NeoPixel(this->width() * this->height(), pin, t);
}

void LEDTable::begin()
{
  strip.begin();
} 

void LEDTable::show()
{
  strip.show();
} 

boolean LEDTable::canShow()
{
  return strip.canShow();
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

Color LEDTable::at(int x, int y)
{
  this->pixelorder(this, &x, &y);
  if ( x < 0 || x >= width() || y < 0 || y >= height()) return color_default;
  return strip.getPixelColor(x + y * width());
}

void LEDTable::ellipse(int x1, int y1, int x2, int y2, Color color)
{
  // TODO: This can overflow if we have more that 128x128 pixel size.
  //       We could use floats but that could be more overhead.
  int x_min = min(x1, x2) - 1;
  int x_max = max(x1, x2);
  int y_min = min(y1, y2) - 1;
  int y_max = max(y1, y2);
  int width = x_max - x_min;
  int height = y_max - y_min;
  int x0_2 = x_max + x_min;
  int y0_2 = y_max + y_min;
  long radius = height;
  radius *= width;
  radius *= radius; // radius = 4 * width * width * height * height
  for (int x = x_min; x < x_max; x++)
  {
    for (int y = y_min; y < y_max; y++)
    {
      long k1 = abs(x+x - x0_2);
      k1 *= height;
      long k2 = abs(y+y - y0_2);
      k2 *= width;
      if (k1*k1 + k2*k2 < radius)
      {
        fill(x, y, color);
      }
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

uint8_t LEDTable::brightness()
{
  return strip.getBrightness();
}
void LEDTable::brightness(uint8_t brightness)
{
  strip.setBrightness(brightness);
}