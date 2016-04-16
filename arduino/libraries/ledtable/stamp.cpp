#include "ledtable.h"

Stamp::Stamp(const uint32_t* lines, uint8_t height, uint8_t width, Color color, Color backgroundColor) : 
    _lines(lines), _width(width), _height(height), _color(color), _backgroundColor(backgroundColor)
{
  if (width > 32)
  {
    computeWidth();
  }
}

Stamp::Stamp(const uint32_t line1, const uint32_t line2, const uint32_t line3, const uint32_t line4, const uint32_t line5, const uint32_t line6, const uint32_t line7, const uint32_t line8)
{
  _color = STAMP_DEFAULT_COLOR;
  _backgroundColor = STAMP_DEFAULT_BACKGROUND_COLOR;
  if      (line8) 
  {
    _height = 8;
  } 
  else if (line7)
  {
    _height = 7;
  }
  else if (line6)
  {
    _height = 6;
  }
  else if (line5)
  {
    _height = 5;
  }
  else if (line4)
  {
    _height = 4;
  }
  else if (line3)
  {
    _height = 3;
  }
  else if (line2)
  {
    _height = 2;
  }
  else if (line1)
  {
    _height = 1;
  }
  else 
  {
    _height = 0;
  }
  uint32_t* lines = (uint32_t*)malloc(sizeof(uint32_t) * _height);
  if (_height >= 1)
  {
    lines[0] = line1;
  }
  if (_height >= 2)
  {
    lines[1] = line2;
  }
  if (_height >= 3)
  {
    lines[2] = line3;
  }
  if (_height >= 4)
  {
    lines[3] = line4;
  }
  if (_height >= 5)
  {
    lines[4] = line5;
  }
  if (_height >= 6)
  {
    lines[5] = line6;
  }
  if (_height >= 7)
  {
    lines[6] = line7;
  }
  if (_height >= 8)
  {
    lines[7] = line8;
  }
  _lines = lines;
  computeWidth();
}

void Stamp::stamp(LEDTable* ledtable, int x0, int y, Color color, Color backgroundColor)
{
  if (color == color_default) 
  { 
    color = this->color();
  }
  if (backgroundColor == color_default) 
  { 
    backgroundColor = this->backgroundColor();
  }
  for (int i = 0; i < height(); i++)
  {
    uint32_t line = this->line(i);
    for (int x = x0 + width() - 1; x >= x0; x--)
    {
      ledtable->fill(x, y, line & 1 ? color : backgroundColor);
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
Color Stamp::backgroundColor()
{
  return _backgroundColor;
}
void Stamp::backgroundColor(Color backgroundColor)
{
  _backgroundColor = backgroundColor;
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

void Stamp::computeWidth()
{
  uint8_t max_width = 0;
  for (int i = 0; i < height(); i++)
  {
    uint8_t current_width = 0;
    uint32_t line = _lines[i];
    while (line)
    {
      line >>= 1;
      current_width++;
    }
    if (current_width > max_width)
    {
      max_width = current_width;
    }
  }
  _width = max_width;
}
