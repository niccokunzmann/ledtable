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
    case 2:
      red = c1;
      green = c2;
      blue = brightness;
  }
  return RGBA(red, green, blue, alpha);
}
