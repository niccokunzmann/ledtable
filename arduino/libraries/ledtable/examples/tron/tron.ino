#include <ledtable.h>

/* These constants can change the game */
#define NUMBER_OF_PLAYERS 4
#define STEPS_TO_HOLE 5
#define BACKGROUND_COLOR color_black
#define TIME_BETWEEN_ROUNDS_MILLIS 300

/* These are private constants */
#define MAXIMUM_NUMBER_OF_PLAYERS 8
#define EFFECT_COLORS 5

LEDTable ledtable = LEDTable(2, 20, 15, PIXELORDER<flip_xy, snake>);

int numberOfPlayers = 0;

#define PINS_PER_PLAYER 2
const int playerPins[MAXIMUM_NUMBER_OF_PLAYERS][PINS_PER_PLAYER] = {{3,4},{5,6},{7,8},{9,10},{11,12},{13,A0},{A1,A2},{A3,A4}};
const Color colors[MAXIMUM_NUMBER_OF_PLAYERS] = {color_red, color_green, color_yellow, color_violet, 
                                                 color_orange, color_darkgreen, color_darkgoldenrod, color_white};

class Effect
{
private:
  const Color colors[EFFECT_COLORS] = {color_black, color_black, color_black, color_black, color_black};
  int x;
  int y;
  uint8_t color_index;
  bool activated; 
public:
  Effect()
  {
    activated = false;
    color_index = 0;
  }

  void activate()
  {
    for (int i = 10; i; i--)
    {
      x = random(ledtable.minX(), ledtable.maxX() + 1);
      y = random(ledtable.minY(), ledtable.maxY() + 1);
      activated = ledtable.at(x, y) == BACKGROUND_COLOR;
      if (activated)
      {
        break;
      }
    }
  }

  void update()
  {
    if (activated)
    {
      paint();
      collide();
    }
  }

  void paint()
  {
    color_index = (color_index + 1) % EFFECT_COLORS;
    ledtable.fill(x, y, colors[color_index]);
  }

  void collide()
  {
    
  }
};


enum PlaterState {MOVING, EXPLODING, END_OF_EXPLOSION, DEAD};
enum PlayerDirection {DOWN = 0, LEFT = 1, UP = 2, RIGHT = 3, STANDING};
enum ButtonState {CATCHING_INPUT, WAITING_FOR_PLAYER_TO_SWITCH_BACK};

class Player
{
private:
  int x;
  int y;
  int last_x;
  int last_y;
  uint8_t index;
  PlayerDirection direction;
  PlayerDirection newDirection;
  unsigned int walked;
  PlaterState state;
  ButtonState inputState;
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
      case MOVING: react(); move(); collide(); paint(); break;
      case EXPLODING: explode(); break;
      case END_OF_EXPLOSION: removeExplosion(); break;
      case DEAD: break;
    }
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

  void collide() 
  {
    if ((ledtable.at(x, y) != BACKGROUND_COLOR) || ledtable.isOutside(x, y))
    {
      state = EXPLODING;
    }
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
    ledtable.fill(last_x, last_y, transparent(BACKGROUND_COLOR, walked % STEPS_TO_HOLE ? 150 : 0));
    ledtable.fill(x, y, transparent(color(), 200));
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
      }
      if (rightPressed())
      {
        newDirection = PlayerDirection((direction + 1) % 4);
      }
    }
  }
};


Player players[8] = { Player(), Player(), Player(), Player(), Player(), Player(), Player(), Player() };

void updatePlayers()
{
  for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
  {
    players[i].update();
  }
}

void setupPlayers()
{
  for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
  {
    players[i].setup();
  }
}

void catchInput()
{
  for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
  {
    players[i].catchInput();
  }
}

int frame;

void setup()
{
  frame = 0;
  ledtable.begin();
  ledtable.fill(BACKGROUND_COLOR);
  //ledtable.brightness(50);
  setupPlayers();
}

void wait()
{
  int now = millis();
  int stop = millis() + TIME_BETWEEN_ROUNDS_MILLIS;
  int offset;
  if (stop < now) 
  {
    offset = - 2 * TIME_BETWEEN_ROUNDS_MILLIS;
    now -= offset;
    stop -= offset;
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
  ledtable.show();
  wait();
  frame++;
}













