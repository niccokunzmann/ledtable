#include "ledtable.h"

LEDTable::LEDTable(int pin,
    int width,
    int height,
    PixelOrder pixelorder,
    neoPixelType strip_type) : _width(width), _height(height), _pixelorder(pixelorder), strip_type(strip_type), pin(pin)
{
  /* determine whether the table is rotated by 90° */
  int x = originalWidth() - 1;
  int y = originalHeight() - 1;
  pixelOrder(&x, &y);
  _widthAndHeightAreSwitched = isOutsideTransformed(x, y);
  /* create the strip */
  strip = NULL;
#ifdef USE_SERIAL_CONNECTION
  printed_pixels = NULL;
#endif
}

LEDTable::~LEDTable()
{
  if (strip)
  {
    delete strip;
  }
#ifdef USE_SERIAL_CONNECTION
  free(printed_pixels);
#endif
}

void LEDTable::begin()
{
  if (!strip)
  {
    strip = new Adafruit_NeoPixel(this->originalWidth() * this->originalHeight(), pin, strip_type);
  }
  if (strip)
  {
    strip->begin();
  }
} 

void LEDTable::show()
{
  if (strip)
  {
    strip->show();
  }
} 

boolean LEDTable::canShow()
{
  return strip && strip->canShow();
}

const bool LEDTable::isOutsideTransformed(const int x, const int y) 
{
  return x < 0 || x >= originalWidth() || y < 0 || y >= originalHeight();
}

void LEDTable::updateColor(uint16_t index, Color color)
{
  if (!strip || color == color_transparent) return;
#ifdef USE_SERIAL_CONNECTION
  if (printed_pixels)
  {
    printed_pixels[index] = strip->getPixelColor(index) != (color & 0xffffff);
  }
#endif 
  if (!(color & 0xff000000)) 
  {
    strip->setPixelColor(index, color);
    return;
  }
  // mixing different transparencies
  uint16_t transparency = ALPHA(color);
  Color color0 = strip->getPixelColor(index);
  uint16_t red = (RED(color) * (256 - transparency) + RED(color0) * transparency) >> 8;
  uint16_t green = (GREEN(color) * (256 - transparency) + GREEN(color0) * transparency) >> 8;
  uint16_t blue = (BLUE(color) * (256 - transparency) + BLUE(color0) * transparency) >> 8;
  strip->setPixelColor(index, RGB(red, green, blue));
}

void LEDTable::updateColorTransformed(int x, int y, Color color)
{
  int index = stripeIndexTransformed(x, y);
  updateColor(index, color);
}

void LEDTable::fill(Color color) 
{
  for (int i = width() * height() - 1; i >= 0; i--) {
    updateColor(i, color);
  }
}

void LEDTable::fill(int x, int y, Color color) 
{
//  Serial.print("(");Serial.print(x);Serial.print(",");Serial.print(y);Serial.print(")");
  pixelOrder(&x, &y);
  if (isOutsideTransformed(x, y)) return;
  updateColorTransformed(x, y, color);
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
  if (!strip)
  {
    return color_default;
  }
  pixelOrder(&x, &y);
  if (isOutsideTransformed(x, y)) return color_default;
  return strip->getPixelColor(x + y * originalWidth()) & 0xffffff;
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

void LEDTable::line(int px, int py, int qx, int qy, Color color)
{
#define FILL(x, y) if (x_and_y_are_switched) { fill(y, x, color); } else { fill(x, y, color); };
#define SWITCH(x, y) ;x-= y; y+= x; x = y - x;

  int x_direction = qx > px ? 1 : -1;
  int y_direction = qy > py ? 1 : -1;
  int dx = qx - px;
  int dy = qy - py;
  bool x_and_y_are_switched = abs(dx) < abs(dy);
  if (x_and_y_are_switched)
  {
    SWITCH(x_direction, y_direction);
    SWITCH(dx, dy);
    SWITCH(px, py);
    SWITCH(qx, qy);
  }
  // after this we know that the slope <= 45°
  if (py == qy)
  {
    for (int x = px; x != qx + x_direction; x+= x_direction)
    {
      FILL(x, py);
    }
    return;
  }
  int last_x = px;
  int last_y = py;
//  Serial.print("dx: "); Serial.print(dx);
//  Serial.print("dy: "); Serial.print(dy);
//  Serial.print("x_direction: "); Serial.print(x_direction);
//  Serial.print("y_direction: "); Serial.print(y_direction);
  long last_y_dx = long(py) * long(dx);
  while (last_x != qx || last_y != qy)
  {
    FILL(last_x, last_y);//Serial.println();
    // compute the new positions
    int new_x = last_x + x_direction;
    int x1 = new_x;
    int y1 = last_y;
    int x2 = new_x;
    int y2 = last_y + y_direction;
    long new_y_dx = long(new_x - px) * long(dy) + long(py) * long(dx);
    int y1_dx = last_y_dx; // = y1 * dx
    int y2_dx = y2 * dx;
    //Serial.print("new_x: "); Serial.print(new_x); Serial.print(" new_y: "); Serial.print(new_y);
    last_x = new_x;
    if (abs(new_y_dx - y1_dx) < abs(new_y_dx - y2_dx))
    {
      last_y = y1;
      last_y_dx = y1_dx;
    } else {
      last_y = y2;
      last_y_dx = y2_dx;
    }
//    Serial.println();
  }
  FILL(qx, qy);
//  Serial.println();

#undef FILL
#undef SWITCH
}

// always use lines with number of points dividable by two to create larger lines
// -> log memory overhead
void LEDTable::line(int x1, int y1, int x2, int y2, int x3, int y3, Color color)
{
  line(x1, y1, x2, y2, color);
  line(x3, y3, x2, y2, color);
}
void LEDTable::line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Color color)
{
  line(x1, y1, x2, y2, color);
  line(x3, y3, x2, y2, color);
  line(x3, y3, x4, y4, color);
}
void LEDTable::line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, Color color)
{
  line(x1, y1, x2, y2, x3, y3, x4, y4, color);
  line(x4, y4, x5, y5, color);
}
void LEDTable::line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, Color color)
{
  line(x1, y1, x2, y2, x3, y3, x4, y4, color);
  line(x4, y4, x5, y5, color);
  line(x6, y6, x5, y5, color);
}
void LEDTable::line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int x7, int y7, Color color)
{
  line(x1, y1, x2, y2, x3, y3, x4, y4, color);
  line(x4, y4, x5, y5, x6, y6, x7, y7, color);
}
void LEDTable::line(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int x7, int y7, int x8, int y8, Color color)
{
  line(x1, y1, x2, y2, x3, y3, x4, y4, color);
  line(x4, y4, x5, y5, x6, y6, x7, y7, color);
  line(x7, y7, x8, y8, color);
}

void LEDTable::print(Text* text, int x, int y, Color text_color, Color background_color) {
  Serial.print("x2: "); Serial.println(x);
  text->printOn(this, x, y, text_color, background_color);
}

const int LEDTable::originalHeight()
{
  return _height;
}

const int LEDTable::originalWidth()
{
  return _width;
}

const int LEDTable::height()
{
  if (_widthAndHeightAreSwitched) return _width;
  return _height;
}

const int LEDTable::width()
{
  if (_widthAndHeightAreSwitched) return _height;
  return _width;
}

uint8_t LEDTable::brightness()
{
  if (!strip)
  {
    return 0;
  }
  return strip->getBrightness();
}
void LEDTable::brightness(uint8_t brightness)
{
  if (strip)
  {
    strip->setBrightness(brightness);
  }
}

const int LEDTable::maxX()
{
  return width() - 1;
}

const int LEDTable::maxY()
{
  return height() - 1;
}

const int LEDTable::minX()
{
  return 0;
}

const int LEDTable::minY()
{
  return 0;
}

const int LEDTable::middleX()
{
  return width() / 2;
}

const int LEDTable::middleY()
{
  return height() / 2;
}

bool LEDTable::isInside(int x, int y)
{
  return !isOutside(x, y);
}

bool LEDTable::isOutside(int x, int y)
{
  pixelOrder(&x, &y);
  return isOutsideTransformed(x, y);
}

void LEDTable::pixelOrder(int* x, int*y)
{
  _pixelorder(this, x, y);
}

int LEDTable::stripeIndex(int x, int y)
{
  pixelOrder(&x, &y);
  return stripeIndexTransformed(x, y);
}

int LEDTable::stripeIndexTransformed(int x, int y)
{
  return x + y * originalWidth();
}


#ifdef USE_SERIAL_CONNECTION
void LEDTable::printToSerial(HardwareSerial* serial)
{
  if (!printed_pixels)
  {
    int number_of_pixels = originalWidth() * originalHeight();
    printed_pixels = (bool*)(malloc(number_of_pixels));
    if (printed_pixels) 
    {
      memset(printed_pixels, true, number_of_pixels);
    }
    serial->println();
  }
  serial->print(SERIAL_COMMAND_CHARACTER);
  serial->print("p ");
  serial->println(height());
  for (int y = 0; y < height(); y++)
  {
    int skipped_pixels = 0;
    for (int x = 0; x < width(); x++)
    {
      skipped_pixels++;
      const int printedPixelsIndex = stripeIndex(x, y);
      Color color = at(x, y);
      if ((!printed_pixels) || (printed_pixels[printedPixelsIndex]))
      {
        for (int i = skipped_pixels ; i > 0 ; i--)
        {
          serial->print("#");
        }
        skipped_pixels = 0;
        serial->print(color & 0xffffff, HEX);
        if (printed_pixels)
        {
          printed_pixels[printedPixelsIndex] = false;
        }
      }
    }
    serial->println();
  }
}

void LEDTable::printPixelOrderToSerial(HardwareSerial* serial)
{
  serial->print(SERIAL_COMMAND_CHARACTER);
  serial->print("o ");
  serial->println(this->originalHeight());
  for (int y = 0; y < originalHeight(); y++)
  {
    for (int x = 0; x < originalWidth(); x++)
    {
      int stripeIndex = this->stripeIndex(x, y);
      if (x != 0)
      {
        serial->print(" ");
      }
      serial->print(stripeIndex);
    }
    serial->println();
  }
}

#endif
