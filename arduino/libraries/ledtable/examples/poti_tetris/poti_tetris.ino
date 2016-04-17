#include <Adafruit_NeoPixel.h>
#include <ledtable.h>

/* POSITION_PIN - attach a potentiometer to that pin
 * ROTATION_PIN - attach a potentiometer to that pin
 * FALL_DOWN_PIN - attach a button to that pin that is connected to ground when pressed
 */
#define POSITION_PIN A0
#define ROTATION_PIN A1
#define FALL_DOWN_PIN 5
#define LEDTABLE_PIN 6

#define ROTATION_POSITIONS_TO_CHOOSE_FROM 7
#define MAXIMUM_ANALOG_READ_VALUE 1023
#define MINIMUM_ANALOG_READ_VALUE 0
#define MILLISECONDS_TO_FALL 100
#define MILLISECONDS_TO_FALL_FAST 30
#define BACKGROUND_COLOR color_black

LEDTable ledtable = LEDTable(LEDTABLE_PIN, 20, 15, PIXELORDER<flip_xy, snake>);

#define MINIMUM_BLOCK_BRIGHTNESS 60
#define MAXIMUM_BACKGROUND_BRIGHTNESS 50 

#define BLOCK_INVISIBLE color_black
#define BLOCK_VISIBLE color_blue


#define DEBUG(VAR) Serial.print(#VAR); Serial.print("= "); Serial.print(VAR); Serial.print(" ");
#define DEBUGLN(VAR) Serial.print(#VAR); Serial.print("= "); Serial.print(VAR); Serial.println();

typedef Stamp Block;

#define NUMBER_OF_BLOCKS 7
#define NUMBER_OF_ROTATIONS 4
#define BLOCK_X_OFFSET 1
#define BLOCK_Y_OFFSET 1
#define BIGGEST_BLOCK_SIZE 4
#define block (blocks[block_number][block_rotation])

Block* blocks[NUMBER_OF_BLOCKS][NUMBER_OF_ROTATIONS];

void allocate_blocks()
{
  Block* block1   = new Block(0b11, 
                              0b11);
  
  Block* block2_1 = new Block(0b000,
                              0b111, 
                              0b010);
  Block* block2_2 = new Block(0b010,
                              0b011, 
                              0b010);
  Block* block2_3 = new Block(0b010,
                              0b111, 
                              0b000);
  Block* block2_4 = new Block(0b010,
                              0b110, 
                              0b010);
  
  Block* block3_1 = new Block(0b010,
                              0b010, 
                              0b010,
                              0b010);
  Block* block3_2 = new Block(0b0000,
                              0b1111);
                              
  Block* block4_1 = new Block(0b011,
                              0b110);
  Block* block4_2 = new Block(0b10,
                              0b11,
                              0b01);
                              
  Block* block5_1 = new Block(0b110,
                              0b011);
  Block* block5_2 = new Block(0b01,
                              0b11,
                              0b10);
  
  Block* block6_1 = new Block(0b110,
                              0b010,
                              0b010);
  Block* block6_2 = new Block(0b001,
                              0b111,
                              0b000);
  Block* block6_3 = new Block(0b010,
                              0b010,
                              0b011);
  Block* block6_4 = new Block(0b000,
                              0b111,
                              0b100);
  
  Block* block7_1 = new Block(0b000,
                              0b111,
                              0b100);
  Block* block7_2 = new Block(0b010,
                              0b010,
                              0b110);
  Block* block7_3 = new Block(0b000,
                              0b111,
                              0b001);
  Block* block7_4 = new Block(0b011,
                              0b010,
                              0b010);

  Block* _blocks[NUMBER_OF_BLOCKS][NUMBER_OF_ROTATIONS] = {
    {block1,   block1,   block1,   block1  }, 
    {block2_1, block2_2, block2_3, block2_4}, 
    {block3_1, block3_2, block3_1, block3_2}, 
    {block4_1, block4_2, block4_1, block4_2}, 
    {block5_1, block5_2, block5_1, block5_2}, 
    {block6_1, block6_2, block6_3, block6_4}, 
    {block7_1, block7_2, block7_3, block7_4}, 
  };
  memcpy(blocks, _blocks, sizeof(Block*) * NUMBER_OF_BLOCKS * NUMBER_OF_ROTATIONS);
}

uint8_t block_number;
uint8_t block_rotation;
int _x;
int _y;

bool block_hits_left;
bool block_hits_right;
bool block_hits_bottom;
bool block_hits_block;
bool block_is_totally_on_top;
bool block_is_still_not_fully_inside;

void setup_blocks()
{
  allocate_blocks();
}

void choose_random_block()
{
  block_number = random(NUMBER_OF_BLOCKS);
  block_rotation = random(NUMBER_OF_ROTATIONS);
  _x = ledtable.middleX();
  _y = ledtable.maxY();
  compute_block_position();
  while (!block_is_totally_on_top)
  {
    _y++;
    compute_block_position();
  }
}

int block_x_offset()
{
  return   blocks[block_number][0]->width() 
         - block->width();
}

int x()
{
  return block_x_offset() + _x - BLOCK_X_OFFSET;
}

int y()
{
  return _y - BLOCK_Y_OFFSET;
}

#define FOR_BLOCK_COORDINATES(STATEMENT)                          \
  {                                                               \
    int ledtable_x;                                               \
    int ledtable_y;                                               \
    int block_x;                                                  \
    int block_y;                                                  \
    Block* _block = block;                                        \
    for (block_x = 0; block_x < _block->width(); block_x ++)      \
    {                                                             \
      ledtable_x = x() + block_x;                                 \
      for(block_y = 0; block_y < _block->height(); block_y ++)    \
      {                                                           \
        ledtable_y = y() + block_y;                               \
        STATEMENT;                                                \
      }                                                           \
    }                                                             \
  }                                                               \

void compute_block_position()
{
  block_hits_left = false;
  block_hits_right = false;
  block_hits_bottom = false;
  block_hits_block = false;
  block_is_totally_on_top = true;
  block_is_still_not_fully_inside = false;
  FOR_BLOCK_COORDINATES(
    if (block->isSet(block_x, block_y))
    {
      if (ledtable_x < ledtable.minX())
      {
        block_hits_left = true;
      }
      if (ledtable_x > ledtable.maxX())
      {
        block_hits_right = true;
      }
      if (ledtable_y < ledtable.minY())
      {
        block_hits_bottom = true;
      }
      if (is_block(ledtable_x, ledtable_y))
      {
        block_hits_block = true;
      }
      if (ledtable_y <= ledtable.maxY())
      {
        block_is_totally_on_top = false;
      }
      if (ledtable_y > ledtable.maxY())
      {
        block_is_still_not_fully_inside = true;
      }
    }
  )
}

void stamp(Color color)
{
  FOR_BLOCK_COORDINATES(
    if (block->isSet(block_x, block_y))
    {
      ledtable.fill(ledtable_x, ledtable_y, color);
    }
  )
}

void fall()
{
  bool choose_new_block_at_the_end = false;
  stamp(BLOCK_INVISIBLE);
  _y--;
  compute_block_position();
  if (block_hits_bottom || block_hits_block)
  {
    _y++;
    if (block_is_still_not_fully_inside)
    {
      restart();
    }
    else
    {
      choose_new_block_at_the_end = true;
    }
  }
  else
  {
    
  }
  stamp(BLOCK_VISIBLE);
  if (choose_new_block_at_the_end)
  {
    new_block();
  }
}

void new_block()
{
  check_lines();
  choose_random_block();
}

void check_lines()
{
  // TODO: optimize quadratic overhead to linear
  for (int y = ledtable.maxY(); y >= ledtable.minY(); y--)
  {
    bool all_are_blocks = true;
    for (int x = ledtable.minX(); x <= ledtable.maxX(); x++)
    {
      if (!is_block(x, y))
      {
        all_are_blocks = false;
        break;
      }
    }
    if (all_are_blocks)
    {
      for (int _y = y; _y < ledtable.maxY(); _y++)
      {
        for (int x = ledtable.minX(); x <= ledtable.maxX(); x++)
        {
          ledtable.fill(x, _y, ledtable.at(x, _y + 1));
        }
      }
      ledtable.fill(ledtable.minX(), ledtable.maxY(), ledtable.maxX() + 1, ledtable.maxY() + 1, BACKGROUND_COLOR);
    }
  }
}

void set_rotation(int position)
{
  stamp(BLOCK_INVISIBLE);
  int old_block_rotation = block_rotation;
  block_rotation = position % NUMBER_OF_ROTATIONS;
  if (block_rotation < 0)
  {
    block_rotation += NUMBER_OF_ROTATIONS;
  }
  compute_block_position();
  if (block_hits_left || block_hits_right || block_hits_bottom || block_hits_block)
  {
    block_rotation = old_block_rotation;
  }
  stamp(BLOCK_VISIBLE);
}

void move(int dx)
{
  stamp(BLOCK_INVISIBLE);
  int old_x = _x;
  _x += dx;
  compute_block_position();
  if (block_hits_left || block_hits_right || block_hits_bottom || block_hits_block)
  {
    _x = old_x;
  }
  stamp(BLOCK_VISIBLE);
}

bool is_background(int x, int y)
{
  return ledtable.isInside(x, y) && ledtable.brightnessAt(x, y) <= MAXIMUM_BACKGROUND_BRIGHTNESS;
}

bool is_block(int x, int y)
{
  return ledtable.isInside(x, y) && ledtable.brightnessAt(x, y) >= MINIMUM_BLOCK_BRIGHTNESS;
}

void restart()
{
  ledtable.fill(BACKGROUND_COLOR);
  choose_random_block();
}

void setup() {
  Serial.begin(9600);
  ledtable.begin();
  setup_blocks();
  setup_falling();
  setup_rotation();
  setup_position();
  restart();
}

unsigned int last_fallen_time;
void setup_falling()
{
  pinMode(FALL_DOWN_PIN, INPUT);
  digitalWrite(FALL_DOWN_PIN, HIGH);
  last_fallen_time = millis();
}
void update_falling()
{
  unsigned int now = millis();
  unsigned int dt = now - last_fallen_time;
  unsigned int dt_to_fall;
  bool fast = digitalRead(FALL_DOWN_PIN) == LOW;
  if (fast)
  {
    dt_to_fall = MILLISECONDS_TO_FALL_FAST;
  }
  else 
  {
    dt_to_fall = MILLISECONDS_TO_FALL;
  }
  if (dt_to_fall <= dt)
  {
    fall();
    last_fallen_time = now;
  }
}
void setup_rotation()
{
  pinMode(ROTATION_PIN, INPUT);
}
void update_rotation()
{
  int position = analogRead(ROTATION_PIN);
  int rotation = map(position, MINIMUM_ANALOG_READ_VALUE, MAXIMUM_ANALOG_READ_VALUE + 1, 0, ROTATION_POSITIONS_TO_CHOOSE_FROM);
  set_rotation(rotation);
}
void setup_position()
{
  pinMode(POSITION_PIN, INPUT);
}
void update_position()
{
  int position = analogRead(POSITION_PIN);
  const int min_position = ledtable.minX() - BIGGEST_BLOCK_SIZE;
  const int max_position = ledtable.maxX() + BIGGEST_BLOCK_SIZE;
  int wanted_position = map(position, MINIMUM_ANALOG_READ_VALUE, MAXIMUM_ANALOG_READ_VALUE + 1, min_position, max_position);
  if (wanted_position < x())
  {
    move(-1);
  }
  else if (wanted_position > x())
  {
    move(1);
  }
}

void loop() {
  update_falling();
  update_rotation();
  update_position();
  ledtable.printToSerial();
  //ledtable.show();
}
