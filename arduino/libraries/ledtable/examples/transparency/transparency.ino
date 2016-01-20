#include <Adafruit_NeoPixel.h>

#include "ledtable.h"

LEDTable ledtable = LEDTable(6, 12, 12, PIXELORDER<snake>);

#define COLORS 8
Color colors[COLORS] = {color_white, color_black, color_red, color_blue, color_green, color_royalblue, color_azure, color_darkviolet};

void setup () {
  ledtable.begin();
  while (true)
  {
    // fill with stripes
    for (int y = 0; y < ledtable.height(); y+= 2)
    {
      ledtable.fill(0, y,     ledtable.width(), y + 1, color_white);
      ledtable.fill(0, y + 1, ledtable.width(), y + 2, color_black);
    }
    ledtable.show();
    // fill colors with transparency
    for (int x = 0; x < ledtable.width(); x++)
    {
      
      int transparency = map(x, ledtable.width() - 1, 0, 0, 255);
      for (int y = 0; y < ledtable.height(); y+= 2)
      {
        Color color = colors[(y/2)%COLORS];
        ledtable.fill(x, y, x + 1, y + 2, transparent(color, transparency));
      }
      ledtable.show();
      delay(500);
    }
  }
  delay(500);
}

void loop() {}
