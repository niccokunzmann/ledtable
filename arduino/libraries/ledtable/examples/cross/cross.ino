#include <ledtable.h>

LEDTable ledtable = LEDTable(6, 12, 12, PIXELORDER<snake>);

void setup() {
  ledtable.begin();
  ledtable.fill(color_black);
  for (int i = 0; i < ledtable.height(); i++) {
    ledtable.fill(i, i, color_blue);
    ledtable.fill(ledtable.height() - i - 1, i, color_green);
  }
  ledtable.show();
}

void loop() {
  
}
