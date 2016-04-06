#include "ledtable.h"

Color random_color(uint8_t brightness, uint8_t alpha)
{
  int c1 = random(int(brightness) + 1);
  int c2 = random(int(brightness) + 1);
  int b = random(3);
  int red;
  int green;
  int blue;
  switch (b)
  {
    case 0:
      red = brightness;
      green = c1;
      blue = c2;
      break;
    case 1:
      red = c1;
      green = brightness;
      blue = c2;
      break;
    case 2:
      red = c1;
      green = c2;
      blue = brightness;
      break;
  }
  return RGBA(red, green, blue, alpha);
}

Color mixColors(Color color2, Color color1, uint8_t strength_of_color2)
{
  uint16_t m = strength_of_color2;
  uint16_t alpha = (ALPHA(color1) * (256 - m) + ALPHA(color2) * m) >> 8;
  uint16_t red = (RED(color1) * (256 - m) + RED(color2) * m) >> 8;
  uint16_t green = (GREEN(color1) * (256 - m) + GREEN(color2) * m) >> 8;
  uint16_t blue = (BLUE(color1) * (256 - m) + BLUE(color2) * m) >> 8;
  return RGBA(red, green, blue, alpha);
}