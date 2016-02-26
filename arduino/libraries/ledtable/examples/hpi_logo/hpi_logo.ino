/*
 * https://hpi.de
 */
 
#include <Adafruit_NeoPixel.h>
#include <ledtable.h>

LEDTable ledtable = LEDTable(6, 20, 15, PIXELORDER<flip_xy, rotate_180, snake>);

//    1   1   1 1   1
//    1 1 1   1 1   1
//    1   1   1     1
const uint32_t HPIbits[3] = {0b10101101, 
                             0b11101101, 
                             0b10101001};
Stamp HPI = Stamp(HPIbits, 8, 3);

void setup() {
  ledtable.begin();
  ledtable.brightness(20);
  ledtable.fill(color_black);
  ledtable.fill(0, 12, 10, 2, RGB(177, 7, 10));
  ledtable.fill(2, 10, 12, 0, RGB(247, 169, 0));
  ledtable.fill(2, 10, 10, 2, RGB(222, 98, 7));
  HPI.stamp(&ledtable, 2, 3);
  ledtable.show();
}

void loop() {
}
