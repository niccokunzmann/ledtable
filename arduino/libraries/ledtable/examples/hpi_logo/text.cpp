#include "ledtable.h"

/* we go row-wise to have variable width
   1 6
   2 7
   3 8
   4 9
   5 ...
   the first 1 bit will be ignored to know what the width is.
 */

uint32_t characterToPixels[LETTERS] = {
    //x1111122222333334444455555
    UNKNOWN_CHARACTER /* '\x00' */,
    UNKNOWN_CHARACTER /* '\x01' */,
    UNKNOWN_CHARACTER /* '\x02' */,
    UNKNOWN_CHARACTER /* '\x03' */,
    UNKNOWN_CHARACTER /* '\x04' */,
    UNKNOWN_CHARACTER /* '\x05' */,
    UNKNOWN_CHARACTER /* '\x06' */,
    UNKNOWN_CHARACTER /* '\x07' */,
    UNKNOWN_CHARACTER /* '\x08' */,
    //x1111122222333334444455555
    0b1000000000000000 /*  '\t'  */,
    0b0 /*  '\n'  */, // needs special treatment
    UNKNOWN_CHARACTER /* '\x0b' */,
    UNKNOWN_CHARACTER /* '\x0c' */,
    0b0 /*  '\r'  */, // needs special treatment
    UNKNOWN_CHARACTER /* '\x0e' */,
    UNKNOWN_CHARACTER /* '\x0f' */,
    UNKNOWN_CHARACTER /* '\x10' */,
    UNKNOWN_CHARACTER /* '\x11' */,
    UNKNOWN_CHARACTER /* '\x12' */,
    UNKNOWN_CHARACTER /* '\x13' */,
    UNKNOWN_CHARACTER /* '\x14' */,
    UNKNOWN_CHARACTER /* '\x15' */,
    UNKNOWN_CHARACTER /* '\x16' */,
    UNKNOWN_CHARACTER /* '\x17' */,
    UNKNOWN_CHARACTER /* '\x18' */,
    UNKNOWN_CHARACTER /* '\x19' */,
    UNKNOWN_CHARACTER /* '\x1a' */,
    UNKNOWN_CHARACTER /* '\x1b' */,
    UNKNOWN_CHARACTER /* '\x1c' */,
    UNKNOWN_CHARACTER /* '\x1d' */,
    UNKNOWN_CHARACTER /* '\x1e' */,
    UNKNOWN_CHARACTER /* '\x1f' */,
    //x1111122222333334444455555
    0b100000 /*  ' '   */,
    0b111101 /*  '!'   */,
    //x1111122222333334444455555
    0b1110000000011000 /*  '"'   */,
    0b10101011111010101111101010 /*  '#'   */,
    0b11110110101111111010110111 /*  '$'   */,
    //x1111122222333334444455555
    0b10001000010001000100010001 /*  '%'   */,
    UNKNOWN_CHARACTER /*  '&'   */,
    0b111000 /*  "'"   */,
    0b10111010001 /*  '('   */,
    0b11000101110 /*  ')'   */,
    UNKNOWN_CHARACTER /*  '*'   */,
    0b1001000111000100 /*  '+'   */,
    //x1111122222333334444455555
    0b10000100010 /*  ','   */,
    0b1001000010000100 /*  '-'   */,
    0b100001 /*  '.'   */,
    0b1000010111010000 /*  '/'   */,
    0b1001100100100110 /*  '0'   */,
    0b1010011111100001 /*  '1'   */,
    //x1111122222333334444455555
    0b1100101010101001 /*  '2'   */,
    0b1101011010111111 /*  '3'   */,
    0b1111000011100100 /*  '4'   */,
    0b1111011010110010 /*  '5'   */,
    0b1011111010110010 /*  '6'   */,
    0b1100001011111000 /*  '7'   */,
    0b1010101010101010 /*  '8'   */,
    0b1111011010111111 /*  '9'   */,
    0b100101 /*  ':'   */,
    //x1111122222333334444455555
    0b0001010 /*  ';'   */,
    UNKNOWN_CHARACTER /*  '<'   */,
    0b1010100101001010 /*  '='   */,
    UNKNOWN_CHARACTER /*  '>'   */,
    0b1100001010111000 /*  '?'   */,
    0b11111110000101111010111111 /*  '@'   */,
    0b1011111010001111 /*  'A'   */,
    0b1111111010101010 /*  'B'   */,
    0b1111111000110001 /*  'C'   */,
    0b1111111000101110 /*  'D'   */,
    0b1111111010110001 /*  'E'   */,
    0b1111111010010000 /*  'F'   */,
    //x1111122222333334444455555
    0b1111101000110011 /*  'G'   */,
    0b1111110010011111 /*  'H'   */,
    0b111111 /*  'I'   */,
    0b1000101000111110 /*  'J'   */,
    0b1111110010011011 /*  'K'   */,
    0b1111110000100001 /*  'L'   */,
    0b11111101000001000100011111 /*  'M'   */,
    //x1111122222333334444455555
    0b111111010000010011111 /*  'N'   */,
    0b101110100011000101110 /*  'O'   */,
    0b1111111010001000 /*  'P'   */,
    0b101110100011001001101 /*  'Q'   */,
    0b1111111011001001 /*  'R'   */,
    0b1010011010110010 /*  'S'   */,
    0b1100001111110000 /*  'T'   */,
    0b1111100000111110 /*  'U'   */,
    0b11100000110000010011011000 /*  'V'   */,
    //x1111122222333334444455555
    0b11111000001000100000111110 /*  'W'   */,
    0b11000101010001000101010001 /*  'X'   */,
    0b1100010101011100 /*  'Y'   */,
    0b1100111010111001 /*  'Z'   */,
    0b11111110001 /*  '['   */,
    0b1100000111000001 /*  '\\'  */,
    0b11000111111 /*  ']'   */,
    0b1010001000001000 /*  '^'   */,
    0b1000010000100001 /*  '_'   */,
    0b11000001000 /*  '`'   */,
    0b1000100010100111 /*  'a'   */,
    0b1111110010100010 /*  'b'   */,
    0b1000100010100101 /*  'c'   */,
    0b1000100010111111 /*  'd'   */,
    0b1001100110101101 /*  'e'   */,
    0b10111110100 /*  'f'   */,
    0b10110101111 /*  'g'   */,
    0b1111110010000011 /*  'h'   */,
    0b101011 /*  'i'   */,
    0b10000110110 /*  'j'   */,
    0b1111110001000101 /*  'k'   */,
    0b111111 /*  'l'   */,
    0b10011100100000110010000011 /*  'm'   */,
    //x1111122222333334444455555
    0b1001110010000011 /*  'n'   */,
    0b1000100010100010 /*  'o'   */,
    0b10111101100 /*  'p'   */,
    0b10110001111 /*  'q'   */,
    0b10001100100 /*  'r'   */,
    0b10010101011 /*  's'   */,
    0b1010001111001001 /*  't'   */,
    //x1111122222333334444455555
    0b1001100000100111 /*  'u'   */,
    0b1001100000100110 /*  'v'   */,
    0b10011000001001110000100110 /*  'w'   */,
    0b0010100001000101 /*  'x'   */,
    0b1010010010101110 /*  'y'   */,
    0b10101101101 /*  'z'   */,
    0b1001000111010001 /*  '{'   */,
    0b11111111111 /*  '|'   */,
    0b1100010111000100 /*  '}'   */,
    0b100010001000001000100 /*  '~'   */,
};

Text::Text(const char* text, Color text_color, Color background_color) : _text(text), _text_color(text_color), _background_color(background_color)
{
  _width = -1;
  _height = -1;
};

const int Text::width(char character)
{
  int character_width = 0;
  uint32_t pixels = getPixels(character);
  while (pixels > 1) {
    character_width += 1;
    pixels >>= CHARACTER_HEIGHT;
  }
  return character_width;
}

const int Text::height(char character)
{
  return CHARACTER_HEIGHT;
};


int Text::height()
{
  return _height;
};

int Text::width()
{
  return _width;
};

const char* Text::text()
{
  return _text;
};

void Text::printOn(LEDTable* ledtable, int x0, int y0, Color text_color, Color background_color)
{
  if (text_color == color_default) {
    text_color = this->text_color();
  }
  if (background_color == color_default) {
    background_color = this->background_color();
  }
  int y = y0;
  int x = x0;
  _width = 0;
  _height = 0;
  Serial.print("x: "); Serial.println(x);
  for (int i = 0; text()[i]; i++) 
  {
    //Serial.print("i: "); Serial.print(int(i));
    //ledtable->fill(x, y, color_green);
    char character = text()[i];
    int character_width = width(character);
    //Serial.print("character: "); Serial.print(int(character));
    //Serial.print(" character_width: "); Serial.print(character_width);
    if (character_width == 0) {
      //ledtable->fill(x, y + 1, color_violet);
      // '\n' or '\r'
      y += CHARACTER_HEIGHT + 1;
      if (x - x0 > _width) _width = x - x0;
      x = x0;
    } else {
      uint32_t pixels = getPixels(character);
      uint32_t pixel_index = 1;
      pixel_index <<= (character_width * CHARACTER_HEIGHT - 1);
      //Serial.print(" INDEX: "); Serial.println(pixel_index);
      while (pixel_index)
      { 
        for (int c = 0; c < CHARACTER_HEIGHT; c++)
        {
          //Serial.print("x: "); Serial.print(x); Serial.print(" y: "); Serial.print(y); Serial.print(" b: ");
          if (pixels & pixel_index) {
            ledtable->fill(x, y + c, text_color);
            //Serial.println("1");
          } else {
            //Serial.println("0");
            ledtable->fill(x, y + c, background_color);
          }
          pixel_index >>= 1;
        }
        //ledtable->fill(x, y + 2, color_red);
        x++;
      }
      for (int c = 0; c < CHARACTER_HEIGHT; c++) 
      {
        ledtable->fill(x, y + c, background_color);
      }
      x++;
    }
  }
  if (x - x0 > _width) _width = x - x0;
  if (_width > 0) _height = CHARACTER_HEIGHT + y - y0;
}

Color Text::text_color()
{
  return _text_color;
};

void Text::text_color(Color text_color)
{
  _text_color = text_color;
};

Color Text::background_color()
{
  return _background_color;
};

void Text::background_color(Color background_color)
{
  _background_color = background_color;
};


