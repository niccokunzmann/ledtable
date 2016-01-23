#include <ledtable.h>

LEDTable ledtable = LEDTable(6, 20, 15, PIXELORDER<snake>);

void setup() {
  ledtable.begin();
  ledtable.brightness(20);
  ledtable.line(0,0, ledtable.width(), ledtable.height(), color_blue);
}

void loop() {
  // https://www.arduino.cc/en/Reference/Random
  int x1 = random(-1, ledtable.width() + 1);
  int y1 = random(-1, ledtable.height() + 1);
  int x2 = random(-1, ledtable.width() + 1);
  int y2 = random(-1, ledtable.height() + 1);
  ledtable.line(x1, y1, x2, y2, ledtable.width() - x1, ledtable.height() - y1, transparent(random_color(), 80));
  ledtable.show();
  delay(800);
}