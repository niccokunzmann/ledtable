#include "ledtable.h"

Stamp::Stamp(const uint32_t* lines, uint8_t width, uint8_t height, Color color, Color background_color) : 
    _lines(lines), _width(width), _height(height), _color(color), _background_color(background_color)
{
}

void Stamp::stamp(LEDTable* ledtable, int x0, int y, Color color, Color background_color)
{
  if (color == color_default) 
  { 
    color = this->color();
  }
  if (background_color == color_default) 
  { 
    background_color = this->background_color();
  }
  for (int i = 0; i < height(); i++)
  {
    uint32_t line = this->line(i);
    for (int x = x0 + width() - 1; x >= x0; x--)
    {
      ledtable->fill(x, y, line & 1 ? color : background_color);
      line >>= 1;
    }
    y++;
  }
  
}

const uint8_t Stamp::height()
{
  return _height;
}
const uint8_t Stamp::width()
{
  return _width;
}
Color Stamp::background_color()
{
  return _background_color;
}
void Stamp::background_color(Color background_color)
{
  _background_color = background_color;
}
Color Stamp::color()
{
  return _color;
}
void Stamp::color(Color color)
{
  _color = color;
}
const uint32_t* Stamp::lines()
{
  return _lines;
}
uint32_t Stamp::line(uint8_t index)
{
  return _lines[index];
}
