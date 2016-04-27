/*
 * https://hpi.de
 */
 
#include <Adafruit_NeoPixel.h>
#include <ledtable.h>

LEDTable ledtable = LEDTable(6, 20, 15, PIXELORDER<rotate_180, snake>);

//    1   1   1 1   1
//    1 1 1   1 1   1
//    1   1   1     1
Stamp HPI = Stamp(0b10101101, 
                  0b11101101, 
                  0b10101001);

void setup() {
  Serial.begin(9600);
  ledtable.begin();
  ledtable.brightness(50);
  ledtable.fill(color_black);
  ledtable.fill(0, 12, 10, 2, RGB(177, 7, 10));
  ledtable.fill(2, 10, 12, 0, RGB(247, 169, 0));
  ledtable.fill(2, 10, 10, 2, RGB(222, 98, 7));
  HPI.stamp(&ledtable, 2, 3);
  ledtable.show();
  ledtable.printToSerial();
  ledtable.printPixelOrderToSerial();
}

void loop() {
}