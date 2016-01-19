#include <Adafruit_NeoPixel.h>

#include "ledtable.h"

LEDTable ledtable = LEDTable(6, 12, 12, PIXELORDER(snake));

void setup() {
  ledtable.begin();
  ledtable.fill(color_black);
  ledtable.fill(0, 12, 10, 2, RGB(177, 7, 58));
  ledtable.fill(2, 10, 12, 0, RGB(247, 169, 0));
  ledtable.fill(2, 10, 10, 2, RGB(222, 98, 7));
  ledtable.show();
}

void loop() {
}
