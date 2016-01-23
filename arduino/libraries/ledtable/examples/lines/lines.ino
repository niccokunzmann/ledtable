#include <ledtable.h>

LEDTable ledtable = LEDTable(6, 12, 12, PIXELORDER<snake>);

void setup() {
  ledtable.begin();
}

void loop() {
  ledtable.fill(color_black);
  ledtable.line(1, 1, 8, 8, color_blue);
  ledtable.line(1, 0, 8, 4, color_red);
  ledtable.line(0, 1, 4, 8, color_green);
  ledtable.line(ledtable.width() - 1, 0, 9, 9, color_yellow);
  ledtable.line(0, ledtable.height() - 1, 9, 9, color_violet);
  ledtable.show();
  delay(3000);
  ledtable.fill(color_black);
  ledtable.line(1,1, 8, 2, color_blue);
  ledtable.line(1,3, 9, 4, color_red);
  ledtable.line(1,5, 10, 6, color_green);
  ledtable.line(1,7, 10, 7, color_violet);
  ledtable.show();
  delay(3000);
  ledtable.fill(color_black);
  ledtable.line(1,1, 2, 8, color_blue);
  ledtable.line(3,1, 4, 9, color_red);
  ledtable.line(5,1, 6, 10, color_green);
  ledtable.line(7,1, 7, 10, color_violet);
  ledtable.show();
  delay(3000);
}
