#include <Adafruit_NeoPixel.h>

#include "ledtable.h"

LEDTable ledtable = LEDTable(6, 12, 12, PIXELORDER<snake>);
Text text = Text("Hello, World!", 0x050505, color_black);

void setup() {
  ledtable.begin();
  ledtable.fill(color_black);
  ledtable.print(&text);
  ledtable.show();
}

int i = 0;

void loop() {
  if (i <= - text.width()) {
    i = ledtable.width();
  }
  ledtable.print(&text, i);
  ledtable.show();
  delay(100);
  i--;
}
