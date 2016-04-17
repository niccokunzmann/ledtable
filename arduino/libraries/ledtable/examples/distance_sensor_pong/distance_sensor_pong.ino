#include <Adafruit_NeoPixel.h>
#include <ledtable.h>
/* Pong
 * ====
 * 
 * The old game controlled by IR sensors
 * 
 * You can change the PLAYER_1_TRIGGER_PIN, ... variables to match the pins where you stick the distance sensors in.
 * LED_STRIPE_PIN is the pin where the LED table is connected to.
 * USE_SERIAL_CONNECTION_INSTEAD_OF_PIN - set to true if yoou do not have an led table and want to program via Python/Serial
 */

#define PLAYER_1_ECHO_PIN 7
#define PLAYER_1_TRIGGER_PIN 8
#define PLAYER_2_ECHO_PIN 9
#define PLAYER_2_TRIGGER_PIN 10

#define LED_STRIPE_PIN 6
LEDTable ledtable = LEDTable(LED_STRIPE_PIN, 20, 14, PIXELORDER<double_spiral>);

#define USE_SERIAL_CONNECTION_INSTEAD_OF_PIN false
#define BALL_VELOCITY_IN_PIXELS_PER_SECOND 20
#define MILLISECONDS_FOR_FRAME 50
#define BACKGROUND_COLOR color_black
#define BALL_COLOR color_white
#define PLAYER_1_COLOR color_red
#define PLAYER_2_COLOR color_blue
#define SENSOR_MIN_MICROSECONDS 300
#define SENSOR_MAX_MICROSECONDS 1600
#define SENSOR_STAND_STILL_MICROSECONDS 4000
#define MAX_COUNT (ledtable.width() / 2 - 1)





// ball
float ball_x;
float ball_y;
float ball_dx;
float ball_dy;
#define BALL_SIZE 1
Color ball_color = BALL_COLOR;

// player 1
float player_1_x;
float player_1_y;
float player_1_size = 3;
bool player_1_does_not_move;
int player_1_count;

// player 2
float player_2_x;
float player_2_y;
float player_2_size = 3;
bool player_2_does_not_move;
int player_2_count;

// time difference
float delta_t;

#define DEBUG(var) Serial.print(#var); Serial.print(": "); Serial.println(var);

long get_ping(const int trigger_pin, const int echo_pin)
{
  // from http://www.arduino.cc/en/Tutorial/Ping?from=Tutorial.UltrasoundSensor
  long duration;
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigger_pin, OUTPUT);
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigger_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echo_pin, INPUT);
  duration = pulseIn(echo_pin, HIGH, SENSOR_STAND_STILL_MICROSECONDS);
  return duration;
}

void player_position() 
{
  long player_1_micros = get_ping(PLAYER_1_TRIGGER_PIN, PLAYER_1_ECHO_PIN);
  long player_2_micros = get_ping(PLAYER_2_TRIGGER_PIN, PLAYER_2_ECHO_PIN);
  if (player_1_micros < SENSOR_STAND_STILL_MICROSECONDS && player_1_micros)
  {
    player_1_y = micros2playery(player_1_micros, player_1_size);
    player_1_does_not_move = false;
  } 
  else
  {
    player_1_does_not_move = true;
  }
  if (player_2_micros < SENSOR_STAND_STILL_MICROSECONDS && player_2_micros)
  {
    player_2_y = micros2playery(player_2_micros, player_2_size);
    player_2_does_not_move = false;
  } 
  else
  {
    player_2_does_not_move = true;
  }
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  // from https://www.arduino.cc/en/Reference/Map
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float micros2playery(long m, float size)
{
  return fmap(constrain(m, SENSOR_MIN_MICROSECONDS, SENSOR_MAX_MICROSECONDS), SENSOR_MIN_MICROSECONDS, SENSOR_MAX_MICROSECONDS, ledtable.minY(), ledtable.maxY() + 1);
}

void start()
{
  delta_t = 0;
  
  player_1_x = ledtable.minX();
  player_1_y = ledtable.middleY();
  player_1_does_not_move = false;
  player_1_count = 0;

  player_2_x = ledtable.maxX();
  player_2_y = ledtable.middleY();
  player_2_does_not_move = false;
  player_2_count = 0;
  
  ledtable.fill(BACKGROUND_COLOR);

  player_scored();
}

void normlize_velocity()
{
  float a = float(BALL_VELOCITY_IN_PIXELS_PER_SECOND) * float(MILLISECONDS_FOR_FRAME) / float(1000) / sqrt(ball_dx * ball_dx + ball_dy * ball_dy);
  ball_dx *= a;
  ball_dy *= a;
}

void player_scored()
{
  ball_x = ledtable.middleX();
  ball_y = ledtable.middleY();
  ball_dx = 10 * random(1) - 5;
  ball_dy = 1;
  normlize_velocity();
  if (player_1_count >= MAX_COUNT || player_2_count >= MAX_COUNT)
  {
    if (player_1_count > player_2_count)
    {
      ball_color = mixColors(PLAYER_1_COLOR, BALL_COLOR, 200);
    }
    else if (player_1_count < player_2_count)
    {
      ball_color = mixColors(PLAYER_2_COLOR, BALL_COLOR, 200);
    }
    else
    {
      ball_color = BALL_COLOR;
    }
    start();
  }
}

void update()
{
  player_position();
  ball_x += ball_dx;
  ball_y += ball_dy;
  if (ball_x < ledtable.minX() + 0.5) 
  { 
    if (player_1_y - player_1_size - BALL_SIZE <= ball_y && ball_y <= player_1_y + player_1_size + BALL_SIZE)
    {
      ball_dx = 1;
      ball_dy = ball_y - player_1_y;
      normlize_velocity();
    } else {
      player_2_count += 1;
      player_scored();
    }
  } 
  else if (ball_x > ledtable.maxX()) 
  { 
    if (player_2_y - player_2_size - BALL_SIZE <= ball_y && ball_y <= player_2_y + player_2_size + BALL_SIZE)
    {
      ball_dx = -1;
      ball_dy = ball_y - player_2_y;
      normlize_velocity();
    } else {
      player_1_count += 1;
      player_scored();
    }
  }
  if (ball_y < ledtable.minY())
  { 
    ball_y = 2 * ledtable.minY() - ball_y; 
    ball_dy *= -1; 
  } 
  else if (ball_y > ledtable.maxY() + 1) 
  { 
    ball_y = 2 * ledtable.maxY() + 2 - ball_y; 
    ball_dy *= -1; 
  }
  // background fading
  ledtable.fill(transparent(BACKGROUND_COLOR, 150));
  // elements
  ledtable.fill(1, 0, player_1_count + 1, 1, mixColors(PLAYER_2_COLOR, BACKGROUND_COLOR));
  ledtable.fill(ledtable.maxX(), 0, ledtable.maxX() - player_2_count, 1, mixColors(PLAYER_1_COLOR, BACKGROUND_COLOR));
  ledtable.fill(player_1_x, ledtable.minY(), player_1_x + 1, ledtable.maxY() + 1, BACKGROUND_COLOR);
  ledtable.fill(player_2_x, ledtable.minY(), player_2_x + 1, ledtable.maxY() + 1, BACKGROUND_COLOR);
  ledtable.line(player_1_x, player_1_y - player_1_size / 2, player_1_x, player_1_y + player_1_size / 2, 
                player_1_does_not_move ? mixColors(PLAYER_1_COLOR, BACKGROUND_COLOR, 150) : PLAYER_1_COLOR);
  ledtable.line(player_2_x, player_2_y - player_2_size / 2, player_2_x, player_2_y + player_2_size / 2, 
                player_2_does_not_move ? mixColors(PLAYER_2_COLOR, BACKGROUND_COLOR, 150) : PLAYER_2_COLOR);
  ledtable.fill(ball_x, ball_y, ball_color);
}

void setup() {
  Serial.begin(9600);
  ledtable.begin();
  start();
}

void loop() {
  unsigned long start = millis();
  // update
  update();
  // show the frame
  if (USE_SERIAL_CONNECTION_INSTEAD_OF_PIN)
  {
    ledtable.printToSerial();
  } else {
    ledtable.show();
  }
  // wait until frame is over
  unsigned long dt = MILLISECONDS_FOR_FRAME;
  unsigned long stop = start + dt;
  unsigned long offset = 0;
  if (stop < start)
  {
    offset = MILLISECONDS_FOR_FRAME;
  }
  while (millis() + offset < stop + offset) {}
  delta_t = float (millis() - start) / 1000;
}
