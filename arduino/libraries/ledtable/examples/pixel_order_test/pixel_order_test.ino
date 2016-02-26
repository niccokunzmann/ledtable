#include <ledtable.h>

#define DELAY 50

LEDTable ledtable = LEDTable(6, 20, 14, PIXELORDER<double_spiral>);

void setup() {
  Serial.begin(9600);
  ledtable.begin();
  ledtable.fill(color_black);
  ledtable.show();
  Serial.println("Los:");
  for (int y = ledtable.minY(); y <= ledtable.maxY(); y += 1)
  {
    for (int x = ledtable.minX(); x <= ledtable.maxX(); x += 1)
    {
      ledtable.fill(x, y, color_purple);
      ledtable.show();
      delay(DELAY);
      ledtable.fill(x, y, color_black);
    }
    Serial.println();
  }
}

void loop() {
}


