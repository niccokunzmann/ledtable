//Code for Music from http://www.instructables.com/id/Nyan-Cat-on-Arduino/

#include <Adafruit_NeoPixel.h>
#include <ledtable.h>
#include "pitches.h"

#define DELAY 50
#define IMAGES 3

LEDTable ledtable = LEDTable(6, 20, 14, PIXELORDER<mirror_horizontally, double_spiral>);

int mImageNumber = 0;
int mLastImage = 1;
int mPosVertical = 0;
int mPosVerticalCat = 0;

short melody[] = {
  //A
  NOTE_FS5,
  NOTE_GS5,
  NOTE_DS5,
  NOTE_DS5,
  REST,
  NOTE_B4,
  NOTE_D5,
  NOTE_CS5,
  NOTE_B4,
  REST,
  NOTE_B4,
  NOTE_CS5,
  NOTE_D5,
  NOTE_D5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_GS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_B4,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_GS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_D5,
  NOTE_DS5,
  NOTE_D5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_D5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_FS5,
  NOTE_GS5,
  NOTE_DS5,
  NOTE_DS5,
  REST,
  NOTE_B4,
  NOTE_D5,
  NOTE_CS5,
  NOTE_B4,
  REST,
  NOTE_B4,
  NOTE_CS5,
  NOTE_D5,
  NOTE_D5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_GS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_B4,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_GS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_D5,
  NOTE_DS5,
  NOTE_D5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_D5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_FS5,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  NOTE_B4,
  NOTE_CS5,
  //end of loop
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_E5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_E5,
  NOTE_DS5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_FS4,
  NOTE_DS4,
  NOTE_E4,
  NOTE_FS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_FS4,
  //
  NOTE_B4,
  NOTE_B4,
  NOTE_AS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_E4,
  NOTE_E5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B4,
  NOTE_AS4,
  //
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_E5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_E5,
  NOTE_DS5,
  NOTE_CS5,
  NOTE_B4,
  NOTE_FS4,
  NOTE_DS4,
  NOTE_E4,
  NOTE_FS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_B4,
  NOTE_CS5,
  NOTE_DS5,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_FS4,
  NOTE_B4,
  NOTE_B4,
  NOTE_AS4,
  NOTE_B4,
  NOTE_FS4,
  NOTE_GS4,
  NOTE_B4,
  NOTE_E5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B4,
  NOTE_CS5

};
byte melodyNoteDurations[] = {
  //a
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  8,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  8,
  //
  8,
  16,
  16,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  16,
  16,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  //
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  //
  8,
  16,
  16,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  16,
  16,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  //
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8,
  8
};
short intro[] = {
  //intro
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B5,
  NOTE_CS6,
  NOTE_DS6,
  NOTE_CS6,
  NOTE_AS5,
  NOTE_B5,
  NOTE_FS5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_FS5,
  NOTE_B5,
  NOTE_CS6,
  NOTE_AS5,
  NOTE_B5,
  NOTE_CS6,
  NOTE_E6,
  NOTE_DS6,
  NOTE_E6,
  NOTE_B5,

};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
byte noteDurations[] = {
  16,
  16,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  8, //13
  8,
  16,
  16,
  8,
  8,
  16,
  16,
  16,
  16,
  16,
  16,
  16,
  16 //25 intro
};

void setup() {
  for (int thisNote = 0; thisNote < 25; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, intro[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }

  Serial.begin(9600);
  ledtable.begin();
  ledtable.brightness(40);
}

void image() {
  ledtable.fill(RGBA(255, 255, 255, 64));
  ledtable.fill(4 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(5 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(6 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(7 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(8 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(9 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(10 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(11 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(12 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(13 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(14 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(15 - mPosVerticalCat, 3 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(3 - mPosVerticalCat, 4 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(4 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(5 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(6 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(7 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(8 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(9 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(10 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(11 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(12 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(13 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(14 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(15 - mPosVerticalCat, 4 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 4 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(1 - mPosVerticalCat, 5 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(3 - mPosVerticalCat, 5 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(4 - mPosVerticalCat, 5 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(5 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(6 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(7 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(8 - mPosVerticalCat, 5 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(9 - mPosVerticalCat, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(10 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(11 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(12 - mPosVerticalCat, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(13 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(14 - mPosVerticalCat, 5 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(15 - mPosVerticalCat, 5 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 5 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(17 - mPosVerticalCat, 5 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(18 - mPosVerticalCat, 5 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(19 - mPosVerticalCat, 5 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(0 - mPosVerticalCat, 6 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(1 - mPosVerticalCat, 6 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(2 - mPosVerticalCat, 6 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(3 - mPosVerticalCat, 6 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(4 - mPosVerticalCat, 6 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(5 - mPosVerticalCat, 6 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(6 - mPosVerticalCat, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(7 - mPosVerticalCat, 6 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(8 - mPosVerticalCat, 6 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(9 - mPosVerticalCat, 6 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(10 - mPosVerticalCat, 6 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(11 - mPosVerticalCat, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(12 - mPosVerticalCat, 6 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(13 - mPosVerticalCat, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(14 - mPosVerticalCat, 6 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(15 - mPosVerticalCat, 6 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 6 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(0 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(1 - mPosVerticalCat, 7 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(2 - mPosVerticalCat, 7 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(3 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(4 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(5 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(6 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(7 - mPosVerticalCat, 7 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(8 - mPosVerticalCat, 7 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(9 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(10 - mPosVerticalCat, 7 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(11 - mPosVerticalCat, 7 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(12 - mPosVerticalCat, 7 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(13 - mPosVerticalCat, 7 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(14 - mPosVerticalCat, 7 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(15 - mPosVerticalCat, 7 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 7 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(17 - mPosVerticalCat, 7 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(18 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(19 - mPosVerticalCat, 7 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(0 - mPosVerticalCat, 8 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(1 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(2 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(3 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(4 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(5 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(6 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(7 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(8 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(9 - mPosVerticalCat, 8 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(10 - mPosVerticalCat, 8 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(11 - mPosVerticalCat, 8 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(12 - mPosVerticalCat, 8 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(13 - mPosVerticalCat, 8 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(14 - mPosVerticalCat, 8 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(15 - mPosVerticalCat, 8 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 8 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(17 - mPosVerticalCat, 8 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(18 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(19 - mPosVerticalCat, 8 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(0 - mPosVerticalCat, 9 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(1 - mPosVerticalCat, 9 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(2 - mPosVerticalCat, 9 - mImageNumber, RGB(0, 0, 0));
  ledtable.fill(3 - mPosVerticalCat, 9 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(4 - mPosVerticalCat, 9 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(5 - mPosVerticalCat, 9 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(6 - mPosVerticalCat, 9 - mImageNumber, RGB(0, 0, 0));
  ledtable.fill(7 - mPosVerticalCat, 9 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(8 - mPosVerticalCat, 9 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(9 - mPosVerticalCat, 9 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(10 - mPosVerticalCat, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(11 - mPosVerticalCat, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(12 - mPosVerticalCat, 9 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(13 - mPosVerticalCat, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(14 - mPosVerticalCat, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(15 - mPosVerticalCat, 9 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 9 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(17 - mPosVerticalCat, 9 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(18 - mPosVerticalCat, 9 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(19 - mPosVerticalCat, 9 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(0 - mPosVerticalCat, 10 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(1 - mPosVerticalCat, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(2 - mPosVerticalCat, 10 - mImageNumber, RGB(0, 0, 0));
  ledtable.fill(3 - mPosVerticalCat, 10 - mImageNumber, RGB(0, 0, 0));
  ledtable.fill(4 - mPosVerticalCat, 10 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(5 - mPosVerticalCat, 10 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(6 - mPosVerticalCat, 10 - mImageNumber, RGB(0, 0, 0));
  ledtable.fill(7 - mPosVerticalCat, 10 - mImageNumber, RGB(0, 0, 0));
  ledtable.fill(8 - mPosVerticalCat, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(9 - mPosVerticalCat, 10 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(10 - mPosVerticalCat, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(11 - mPosVerticalCat, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(12 - mPosVerticalCat, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(13 - mPosVerticalCat, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(14 - mPosVerticalCat, 10 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(15 - mPosVerticalCat, 10 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 10 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(17 - mPosVerticalCat, 10 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(18 - mPosVerticalCat, 10 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(19 - mPosVerticalCat, 10 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(0 - mPosVerticalCat, 11 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(1 - mPosVerticalCat, 11 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(2 - mPosVerticalCat, 11 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(3 - mPosVerticalCat, 11 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(4 - mPosVerticalCat, 11 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(5 - mPosVerticalCat, 11 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(6 - mPosVerticalCat, 11 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(7 - mPosVerticalCat, 11 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(8 - mPosVerticalCat, 11 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(9 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(10 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(11 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(12 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(13 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(14 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(15 - mPosVerticalCat, 11 - mImageNumber, RGB(217, 187, 123));
  ledtable.fill(16 - mPosVerticalCat, 11 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(17 - mPosVerticalCat, 11 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(18 - mPosVerticalCat, 11 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(19 - mPosVerticalCat, 11 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(0 - mPosVerticalCat, 12 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(1 - mPosVerticalCat, 12 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(2 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(3 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(4 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(5 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(6 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(7 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(8 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(9 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(10 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(11 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(12 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(13 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(14 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(15 - mPosVerticalCat, 12 - mImageNumber, RGB(102, 51, 0));
  ledtable.fill(16 - mPosVerticalCat, 12 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(17 - mPosVerticalCat, 12 - mImageNumber, RGB(181, 181, 181));
  ledtable.fill(18 - mPosVerticalCat, 12 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(19 - mPosVerticalCat, 12 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(3 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(4 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(5 - mPosVerticalCat, 13 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(6 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(13 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(14 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(16 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(17 - mPosVerticalCat, 13 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(28 - mPosVertical, 4 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(29 - mPosVertical, 4 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(30 - mPosVertical, 4 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(25 - mPosVertical, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(26 - mPosVertical, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(27 - mPosVertical, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(28 - mPosVertical, 5 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(29 - mPosVertical, 5 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(30 - mPosVertical, 5 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(31 - mPosVertical, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(32 - mPosVertical, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(33 - mPosVertical, 5 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(22 - mPosVertical, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(23 - mPosVertical, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(24 - mPosVertical, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(25 - mPosVertical, 6 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(26 - mPosVertical, 6 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(27 - mPosVertical, 6 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(28 - mPosVertical, 6 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(29 - mPosVertical, 6 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(30 - mPosVertical, 6 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(31 - mPosVertical, 6 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(32 - mPosVertical, 6 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(33 - mPosVertical, 6 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(34 - mPosVertical, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(35 - mPosVertical, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(36 - mPosVertical, 6 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(20 - mPosVertical, 7 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(21 - mPosVertical, 7 - mImageNumber, RGB(255, 0, 0));
  ledtable.fill(22 - mPosVertical, 7 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(23 - mPosVertical, 7 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(24 - mPosVertical, 7 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(25 - mPosVertical, 7 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(26 - mPosVertical, 7 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(27 - mPosVertical, 7 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(28 - mPosVertical, 7 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(29 - mPosVertical, 7 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(30 - mPosVertical, 7 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(31 - mPosVertical, 7 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(32 - mPosVertical, 7 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(33 - mPosVertical, 7 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(34 - mPosVertical, 7 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(35 - mPosVertical, 7 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(36 - mPosVertical, 7 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(37 - mPosVertical, 7 - mImageNumber, RGBA(255, 0, 0, 64));
  ledtable.fill(38 - mPosVertical, 7 - mImageNumber, RGBA(255, 0, 0, 128));
  ledtable.fill(39 - mPosVertical, 7 - mImageNumber, RGBA(255, 0, 0, 255));
  ledtable.fill(20 - mPosVertical, 8 - mImageNumber, RGB(98, 99, 97));
  ledtable.fill(21 - mPosVertical, 8 - mImageNumber, RGB(248, 109, 21));
  ledtable.fill(22 - mPosVertical, 8 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(23 - mPosVertical, 8 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(24 - mPosVertical, 8 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(25 - mPosVertical, 8 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(26 - mPosVertical, 8 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(27 - mPosVertical, 8 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(28 - mPosVertical, 8 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(29 - mPosVertical, 8 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(30 - mPosVertical, 8 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(31 - mPosVertical, 8 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(32 - mPosVertical, 8 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(33 - mPosVertical, 8 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(34 - mPosVertical, 8 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(35 - mPosVertical, 8 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(36 - mPosVertical, 8 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(37 - mPosVertical, 8 - mImageNumber, RGBA(248, 109, 21, 64));
  ledtable.fill(38 - mPosVertical, 8 - mImageNumber, RGBA(248, 109, 21, 128));
  ledtable.fill(39 - mPosVertical, 8 - mImageNumber, RGBA(248, 109, 21, 255));
  ledtable.fill(20 - mPosVertical, 9 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(21 - mPosVertical, 9 - mImageNumber, RGB(238, 243, 12));
  ledtable.fill(22 - mPosVertical, 9 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(23 - mPosVertical, 9 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(24 - mPosVertical, 9 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(25 - mPosVertical, 9 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(26 - mPosVertical, 9 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(27 - mPosVertical, 9 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(28 - mPosVertical, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(29 - mPosVertical, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(30 - mPosVertical, 9 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(31 - mPosVertical, 9 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(32 - mPosVertical, 9 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(33 - mPosVertical, 9 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(34 - mPosVertical, 9 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(35 - mPosVertical, 9 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(36 - mPosVertical, 9 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(37 - mPosVertical, 9 - mImageNumber, RGBA(238, 243, 12, 64));
  ledtable.fill(38 - mPosVertical, 9 - mImageNumber, RGBA(238, 243, 12, 128));
  ledtable.fill(39 - mPosVertical, 9 - mImageNumber, RGBA(238, 243, 12, 255));
  ledtable.fill(20 - mPosVertical, 10 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(21 - mPosVertical, 10 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(22 - mPosVertical, 10 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(23 - mPosVertical, 10 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(24 - mPosVertical, 10 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(25 - mPosVertical, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(26 - mPosVertical, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(27 - mPosVertical, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(28 - mPosVertical, 10 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(29 - mPosVertical, 10 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(30 - mPosVertical, 10 - mImageNumber, RGB(255, 255, 255));
  ledtable.fill(31 - mPosVertical, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(32 - mPosVertical, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(33 - mPosVertical, 10 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(34 - mPosVertical, 10 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(35 - mPosVertical, 10 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(36 - mPosVertical, 10 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(37 - mPosVertical, 10 - mImageNumber, RGB(149, 185, 11));
  ledtable.fill(38 - mPosVertical, 10 - mImageNumber, RGBA(149, 185, 11, 64));
  ledtable.fill(39 - mPosVertical, 10 - mImageNumber, RGBA(149, 185, 11, 128));
  ledtable.fill(20 - mPosVertical, 11 - mImageNumber, RGBA(135, 192, 234, 255));
  ledtable.fill(21 - mPosVertical, 11 - mImageNumber, RGB(135, 192, 234));
  ledtable.fill(22 - mPosVertical, 11 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(23 - mPosVertical, 11 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(24 - mPosVertical, 11 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(34 - mPosVertical, 11 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(35 - mPosVertical, 11 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(36 - mPosVertical, 11 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(37 - mPosVertical, 11 - mImageNumber, RGBA(135, 192, 234, 64));
  ledtable.fill(38 - mPosVertical, 11 - mImageNumber, RGBA(135, 192, 234, 128));
  ledtable.fill(39 - mPosVertical, 11 - mImageNumber, RGBA(135, 192, 234, 255));
  ledtable.fill(20 - mPosVertical, 12 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(21 - mPosVertical, 12 - mImageNumber, RGB(237, 193, 210));
  ledtable.fill(37 - mPosVertical, 12 - mImageNumber, RGBA(237, 193, 210, 64));
  ledtable.fill(38 - mPosVertical, 12 - mImageNumber, RGBA(237, 193, 210, 128));
  ledtable.fill(39 - mPosVertical, 12 - mImageNumber, RGBA(237, 193, 210, 255));

}

void loop() {

  int timeold = 0;
  int time;
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 216; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / melodyNoteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
    time = millis() - timeold;
    if (time > 160)
    {
      Serial.println(time);
      timeold = millis();


      image();
      ledtable.show();
      if (mLastImage < mImageNumber && mImageNumber < IMAGES) {
        mLastImage = mImageNumber;
        mImageNumber++;
      } else if (mLastImage < mImageNumber) {
        mLastImage = mImageNumber;
        mImageNumber--;
      } else if (mLastImage > mImageNumber && mImageNumber > 0) {
        mLastImage = mImageNumber;
        mImageNumber--;
      } else if (mLastImage > mImageNumber) {
        mLastImage = mImageNumber;
        mImageNumber++;
      }
      mPosVertical++;
      if (mPosVertical == 40) {
        mPosVertical = 0;
      }
      mPosVerticalCat++;
      if (mPosVerticalCat == 20) {
        mPosVerticalCat = -20;
      }

    }
  }
}
