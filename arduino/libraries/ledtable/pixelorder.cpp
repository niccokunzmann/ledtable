#include "ledtable.h"

/*
 * We suppose that (0,0) is inside and
 * either (originalHeight() - 1, originalWidth() - 1)
 *     or (originalWidth() - 1, originalHeight() - 1)
 * are inside.
 * 
 * use originalWidth() and originalHeight()
 */


void id(LEDTable* ledtable, int*x, int*y) 
{
}

void snake(LEDTable* ledtable, int*x, int*y) 
{
  if ((*y) & 1) {
    *x = ledtable->originalWidth() - *x - 1;
  }
}

void mirror_horizontally(LEDTable* ledtable, int*x, int*y) 
{
  *y = ledtable->originalHeight() - 1 - *y;
}

void mirror_vertically(LEDTable* ledtable, int*x, int*y) 
{
  *x = ledtable->originalHeight() - 1 - *x;
}

void rotate_270(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->originalWidth() - *y - 1; // todo: is that correct?
  *y = _x;
}

void rotate_180(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = ledtable->originalWidth() - *x - 1;
  *y = ledtable->originalHeight() - *y - 1;
}

void rotate_90(LEDTable* ledtable, int*x, int*y) 
{
  int _x = *x;
  *x = *y;
  *y = ledtable->originalHeight() - _x - 1; // todo: is that correct?
}

void flip_xy(LEDTable* ledtable, int*x, int*y)
{
  int t = *x;
  *x = *y;
  *y = t;
}

#define BOX_TYPE_Y0      0  // 0
#define BOX_TYPE_XW_Y0   4  // 4
#define BOX_TYPE_XW      4  // 8
#define BOX_TYPE_XW_YH   4  // 12
#define BOX_TYPE_YH      8  // 16
#define BOX_TYPE_X0_YH  12  // 20
#define BOX_TYPE_X0     12  // 24
#define BOX_TYPE_X0_Y1  12  // 28

// note that we could save a lot of bytes here 
// at the cost of clarity and maintainability
const uint8_t inner_index_lookup_table[16] = {
  // yx 00  01  10  11 Y0
         0,  3,  1,  2,
  // yx 00  01  10  11 XW_Y0
         0,  1,  3,  2,
  // yx 00  01  10  11 XW
//         0,  1,  3,  2,
  // yx 00  01  10  11 XW_YH
//         0,  1,  3,  2,
  // yx 00  01  10  11 YH
         2,  1,  3,  0,
  // yx 00  01  10  11 X0_YH
         2,  3,  1,  0,
  // yx 00  01  10  11 X0
//         2,  3,  1,  0,
  // yx 00  01  10  11 X0_Y1
//         2,  3,  1,  0,
};

void double_spiral(LEDTable* ledtable, int*_x, int*_y)
{
  const int x = *_x;
  const int y = *_y;
  if (ledtable->isOutsideTransformed(x, y))
  {
    *_x = -1;
    *_y = 0;
    return;
  }
  // spiral boxes
  const int box_x = x / 2;
  const int box_y = y / 2;
  const int w = ledtable->originalWidth();
  const int h = ledtable->originalHeight();
  const int box_w = w / 2;
  const int box_h = h / 2;

  const int m = min(min(box_x, box_y), min(box_w - 1 - box_x, box_h - 1 - box_y));
  const int outer = m * 2 * (box_w + box_h - 2 * m);
  const int _box_x = box_x - m;
  const int _box_y = box_y - m;
  const int _box_w = box_w - m - m;
  const int _box_h = box_h - m - m;
  int box_index = outer;

  // inside the box
  bool Y0 = _box_y == 0;
  bool Y1 = _box_y == 1;
  bool X0 = _box_x == 0;
  bool XW = _box_x == _box_w - 1;
  bool YH = _box_y == _box_h - 1;
  uint8_t box_type;

  if (Y0)
  {
    box_index += _box_x;
    if (XW)
    {
      box_type = BOX_TYPE_XW_Y0;
    } else {
      box_type = BOX_TYPE_Y0;
    }
  } else if (XW)
  {
    box_index += _box_w + _box_y - 1;
    if (YH)
    {
      box_type = BOX_TYPE_XW_YH;
    } else
    {
      box_type = BOX_TYPE_XW;
    }
  } else if (YH)
  {
    box_index += _box_w + _box_h + _box_w - 3 - _box_x;
    if (X0)
    {
      box_type = BOX_TYPE_X0_YH;
    } else
    {
      box_type = BOX_TYPE_YH;
    }
  } else if (X0)
  {
    box_index += _box_w + _box_h + _box_w + _box_h - 4 - _box_y;
    if (Y1)
    {
      box_type = BOX_TYPE_X0_Y1;
    } else
    {
      box_type = BOX_TYPE_X0;
    }
  } else
  {
    // ERROR!!!!!
  }
  const uint8_t inner_index = inner_index_lookup_table[box_type + ((y&1)<<1) + (x&1)];
  const int index = inner_index + (box_index << 2);
  *_x = index % ledtable->originalWidth();
  *_y = index / ledtable->originalWidth();
}
