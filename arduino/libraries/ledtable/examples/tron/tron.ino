#include <Adafruit_NeoPixel.h>
#include <ledtable.h>
#include <EEPROM.h>
/*
 * Tron - A Game on the LEDTable
 * 
 * Pins
 *     2 - The green cable of the LEDTable
 *     3, 4   - player 1
 *     5, 6   - player 2
 *     7, 8   - player 3
 *     9, 10  - player 4
 *     11, 12 - player 5
 *     13, A0 - player 6
 *     A1, A2 - player 7
 *     A3, A4 - player 8
 *     
 *   Connect the pins to a switch and connect the switch to ground, GND.
 *     3______/ ____GND
 *     
 * The following constancs can be changed:
 *   NUMBER_OF_PLAYERS - the number of players (0, 2, 3, 4, 5, 6, 7, 8)
 *   STEPS_TO_HOLE     - after how many steps there will be a hole in the line
 *   BACKGROUND_COLOR  - the background color of the LEDTable
 *   TIME_BETWEEN_ROUNDS_MILLIS          - how long to wait between frames (milliseconds)
 *   TIME_BETWEEN_ROUNDS_MILLIS_SPEEDUP  - how long to wait between frames if a speedup powerup is taken (milliseconds)
 *   TIME_BETWEEN_ROUNDS_MILLIS_SLOWDOWN - how long to wait between frames if a slowdown powerup is taken (milliseconds)
 *   NUMBER_OF_FRAMES_TO_BE_INVULNERABLE - number of steps before the player becomes vulnerable again, if the invulnerability powerup was taken
 *   EXPECTED_TIME_BETWEEN_POWERUPS      - the expected time to wait between powerups showing up
 *   SPEEDUP_COLOR      - the color of the speedup powerup
 *   SLOWDOWN_COLOR     - the color of the slowdown powerup
 *   INVULNERABLE_COLOR - the color of the invulnerability powerup
 */
#define NUMBER_OF_PLAYERS 4
#define STEPS_TO_HOLE 5
#define BACKGROUND_COLOR color_black
#define TIME_BETWEEN_ROUNDS_MILLIS 300
#define TIME_BETWEEN_ROUNDS_MILLIS_SPEEDUP 200
#define TIME_BETWEEN_ROUNDS_MILLIS_SLOWDOWN 200
#define NUMBER_OF_FRAMES_TO_BE_INVULNERABLE 10
#define EXPECTED_TIME_BETWEEN_POWERUPS 7

#define SPEEDUP_COLOR color_blue
#define SLOWDOWN_COLOR color_orange
#define INVULNERABLE_COLOR color_white
LEDTable ledtable = LEDTable(2, 20, 15, PIXELORDER<flip_xy, snake>);

/* These are private constants. Do not change! */
#define MAXIMUM_NUMBER_OF_PLAYERS 8

int numberOfPlayers = 0;
unsigned int frame;

#define PINS_PER_PLAYER 2
const int playerPins[MAXIMUM_NUMBER_OF_PLAYERS][PINS_PER_PLAYER] = {{3,4},{5,6},{7,8},{9,10},{11,12},{13,A0},{A1,A2},{A3,A4}};
const Color colors[MAXIMUM_NUMBER_OF_PLAYERS] = {color_red, color_green, color_yellow, color_violet, 
                                                 color_orange, color_darkgreen, color_darkgoldenrod, color_white};

enum PlaterState {MOVING, EXPLODING, END_OF_EXPLOSION, DEAD, INVULNERABLE};
enum PlayerDirection {DOWN = 0, LEFT = 1, UP = 2, RIGHT = 3, STANDING};
enum ButtonState {CATCHING_INPUT, WAITING_FOR_PLAYER_TO_SWITCH_BACK};

void letOtherPlayersCollide(int index);
void speedup();
void slowdown();

class Player
{
private:
  int x;
  int y;
  int last_x;
  int last_y;
  uint8_t index;
  int8_t nextFillIsBlack;
  PlayerDirection direction;
  PlayerDirection newDirection;
  unsigned int walked;
  PlaterState state;
  ButtonState inputState;
  unsigned int frame_to_switch_moving;
public:
  Player()
  {
    index = numberOfPlayers;
    numberOfPlayers++;
    switch (index)
    {
      case 0: x = 0; y = 0; direction = LEFT; break;
      case 1: x = ledtable.maxX(); y = 0; direction = UP; break;
      case 2: x = 0; y = ledtable.maxY(); direction = DOWN; break;
      case 3: x = ledtable.maxX(); y = ledtable.maxY(); direction = RIGHT; break;
      case 4: x = ledtable.middleX() - 1; y = ledtable.middleY(); direction = RIGHT; break; 
      case 5: x = ledtable.middleX() + 1; y = ledtable.middleY(); direction = LEFT; break;
      case 6: x = ledtable.middleX(); y = ledtable.middleY() - 1; direction = DOWN; break;
      case 7: x = ledtable.middleX(); y = ledtable.middleY() + 1; direction = UP; break;
    }
    last_x = x;
    last_y = y;
    walked = 0;
    state = MOVING;
    newDirection = direction;
    inputState = CATCHING_INPUT;
    pinMode(rightPin(), INPUT);
    digitalWrite(rightPin(), HIGH);
    pinMode(leftPin(), INPUT);
    digitalWrite(leftPin(), HIGH);
    frame_to_switch_moving = 0;
    nextFillIsBlack = -1;
  }

  const Color color()
  {
    return colors[index];
  }

  const int leftPin()
  {
    return playerPins[index][0];
  }

  const int rightPin()
  {
    return playerPins[index][1];
  }

  bool leftPressed()
  {
    return digitalRead(leftPin()) == LOW;
  }

  bool rightPressed()
  {
    return digitalRead(rightPin()) == LOW;
  }

  void setup()
  {
    paint();
  }

  void move()
  {
    last_x = x;
    last_y = y;
    switch (direction)
    {
      /* 0 = down, 1 = left, 2 = up, 3  = right, 4 = standing */
      case 0: y--; break;
      case 1: x++; break;
      case 2: y++; break;
      case 3: x--; break;
      case 4: break;
    }
    walked++;
  }

  void update()
  {
    switch (state)
    {
      case MOVING: react(); move(); collideWithPlayers(); collideWithObstaclesAndPowerups(); paint(); break;
      case INVULNERABLE: react(); move(); invulnerablePosition(); switchToMovingIfPowerupIsOver(); paint(); break;
      case EXPLODING: explode(); break;
      case END_OF_EXPLOSION: removeExplosion(); break;
      case DEAD: break;
    }
  }

  void steppedOnPowerup()
  {
    nextFillIsBlack = 1;
  }

  void switchToMovingIfPowerupIsOver()
  {
    if (frame == frame_to_switch_moving)
    {
      state = MOVING;
    }
  }

  void collideWithPlayers()
  {
    letOtherPlayersCollide(index);
  }

  bool collidesWith(Player* player)
  {
    return state == MOVING && x_position() == player->x_position() && y_position() == player->y_position();
  }

  void invulnerablePosition()
  {
    x = (x + ledtable.width()) % ledtable.width();
    y = (y + ledtable.height()) % ledtable.height();
  }

  void removeExplosion()
  {
    ledtable.fill(x - 1, y - 1, x + 2, y + 2, BACKGROUND_COLOR);
    state = DEAD;
    x = -100;
    y = -100;
  }

  void explode() {
    ledtable.fill(x - 1, y - 1, x + 2, y + 2, transparent(color(), 120));
    state = END_OF_EXPLOSION;
  }

  void collideWithObstaclesAndPowerups() 
  {
    if (ledtable.isOutside(x, y))
    {
      collided();
      return;
    }
    Color collided_color = ledtable.at(x, y);
    switch (collided_color)
    {
      case BACKGROUND_COLOR: break;
      case INVULNERABLE_COLOR: becomeInvulnerable(); break;
      case SPEEDUP_COLOR: speedup(); steppedOnPowerup(); break;
      case SLOWDOWN_COLOR: slowdown(); steppedOnPowerup(); break;
      default: collided();
    }
  }

  void collided()
  {
    state = EXPLODING;
  }

  void react()
  {
    if (inputState == CATCHING_INPUT)
    {
      direction = newDirection;
      inputState = WAITING_FOR_PLAYER_TO_SWITCH_BACK;
    }
  }

  void paint()
  {
    Color head = transparent(color(), 200);
    Color tail = transparent(BACKGROUND_COLOR, walked % STEPS_TO_HOLE ? 150 : 0);
    if (state == INVULNERABLE)
    {
      ledtable.fill(last_x, last_y, BACKGROUND_COLOR);
      ledtable.fill(x, y, frame_to_switch_moving - frame > 3 ? INVULNERABLE_COLOR : head);
    } else {
      ledtable.fill(last_x, last_y, tail);
      ledtable.fill(x, y, head);
    }
    if (nextFillIsBlack >= 0) 
    {
      if (nextFillIsBlack == 0)
      {
        ledtable.fill(last_x, last_y, BACKGROUND_COLOR);
        nextFillIsBlack = false;
      }
      nextFillIsBlack--;
    }
  }

  void catchInput()
  {
    if (inputState == WAITING_FOR_PLAYER_TO_SWITCH_BACK && !rightPressed() && !leftPressed())
    {
      inputState = CATCHING_INPUT;
    }
    if (inputState == CATCHING_INPUT)
    {
      if (leftPressed())
      {
        newDirection = PlayerDirection((direction + 3) % 4);
        random(100); random(1223); // create more randomness through players;
      }
      if (rightPressed())
      {
        newDirection = PlayerDirection((direction + 1) % 4);
        random(8);
      }
    }
  }

  void becomeInvulnerable()
  {
    if (state == MOVING || state == INVULNERABLE)
    {
      state = INVULNERABLE;
      frame_to_switch_moving = frame + NUMBER_OF_FRAMES_TO_BE_INVULNERABLE;
    }
  }

  const int x_position()
  {
    return x;
  }

  const int y_position()
  {
    return y;
  }
};


Player players[8] = { Player(), Player(), Player(), Player(), Player(), Player(), Player(), Player() };

#define FOR_PLAYERS(i) for (int i = 0; i < NUMBER_OF_PLAYERS; i++)


void updatePlayers()
{
  FOR_PLAYERS(i)
  {
    players[i].update();
  }
}

void setupPlayers()
{
  FOR_PLAYERS(i)
  {
    players[i].setup();
  }
}

void catchInput()
{
  FOR_PLAYERS(i)
  {
    players[i].catchInput();
  }
}


void letOtherPlayersCollide(int index)
{
  bool collided = false;
  for (int i = index + 1; i < NUMBER_OF_PLAYERS; i++)
  {
    if (players[i].collidesWith(&players[index]) && players[index].collidesWith(&players[i]))
    {
      players[i].collided();
      collided = true;
    }
  }
  if (collided)
  {
    players[index].collided();
  }
}

/********************** Powerups ************************/

int time_between_frames;

void speedup()
{
  if (time_between_frames == TIME_BETWEEN_ROUNDS_MILLIS_SLOWDOWN)
  {
    time_between_frames = TIME_BETWEEN_ROUNDS_MILLIS;
  } else {
    time_between_frames = TIME_BETWEEN_ROUNDS_MILLIS_SPEEDUP;
  }
}

void slowdown()
{
  if (time_between_frames == TIME_BETWEEN_ROUNDS_MILLIS_SPEEDUP)
  {
    time_between_frames = TIME_BETWEEN_ROUNDS_MILLIS;
  } else {
    time_between_frames = TIME_BETWEEN_ROUNDS_MILLIS_SLOWDOWN;
  }
}


#define POWERUP_COLORS 3

Color powerup_colors[POWERUP_COLORS] = { SPEEDUP_COLOR, INVULNERABLE_COLOR, SLOWDOWN_COLOR };

void spawnPowerup()
{
  if (random(0, EXPECTED_TIME_BETWEEN_POWERUPS) == 0)
  {
      int x = random(ledtable.minX(), ledtable.maxX() + 1);
      int y = random(ledtable.minY(), ledtable.maxY() + 1);
      if (ledtable.at(x, y) == BACKGROUND_COLOR)
      {
        int choice = random(0, POWERUP_COLORS);
        ledtable.fill(x, y, powerup_colors[choice]);
      }
  }
}

void setup()
{
  // randomness
  uint8_t v = EEPROM.read(0);
  for (int i = 0; i < v; i++)
  {
    random(256);
  }
  EEPROM.write(0, random(256));
  frame = 1;
  time_between_frames = TIME_BETWEEN_ROUNDS_MILLIS;
  Serial.begin(9600);
  ledtable.begin();
  ledtable.fill(BACKGROUND_COLOR);
  //ledtable.brightness(50);
  setupPlayers();
}

void wait()
{
  int now = millis();
  int stop = millis() + time_between_frames;
  int offset;
  if (stop < now) 
  {
    offset = - 2 * time_between_frames;
    now += offset;
    stop += offset;
  } else  {
    offset = 0;
  }
  for (int i = millis(); i + offset < stop; i = millis())
  {
    catchInput();
  }
}

void loop()
{
  updatePlayers();
  spawnPowerup();
  ledtable.show();
  wait();
  frame++;
}













