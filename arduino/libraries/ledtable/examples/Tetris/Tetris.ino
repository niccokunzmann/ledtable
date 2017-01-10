#include <Snes.h>
#include <Adafruit_NeoPixel.h>
#include <ledtable.h>

/*
 * SNES Controls for Tetris
 * 
 * A:         put the block down at this column
 * START:     start a new round, whole game resets all stats except the sessionHighscore, also shows the highscore and your final score
 * SELECT:    pauses the game
 * UP:        turn the block used at the moment
 * RIGHT:     put the block one pixel to the right
 * LEFT:      put the block one pixel to the left
 * DOWN:      put the pixel one block down
 * 
 */



// Ports for SNES Controller
#define clock 2
#define strobe 3
#define data 4

// Size and pin for the LEDTable
#define sizeX 20
#define sizeY 10
#define LEDTABLE_PIN 6

// Global data storage for the block used at the moment and the next block
byte mThisBlock;
byte mNextBlock;

// Rotation used at the moment (initializes at 0 every game)
short mRotation = 0;

// Start position of blocks (initializes at 0 | 5 every game)
short mPosX = 0;
short mPosY = 5;

// Level management (cycles are the number of blocks fallen, counter is the variable of how many frames are gone since last reset to limit)
byte cycles = 0;
int counter = 0;

// Level management. These can be changed in order to make levels easier or harder:
  // limit - the base number of frames until a block falls used at level 1 (Default 30)
  // maxCycles - the number of blocks that have to fall until the level gets up (Default 10)
  // difficulty - the number of frames until a block falls used at maximum level (Default 10)
  // difficultyHardening - the reduction in number of frames until a block falls when cycles reaches maxCycles (Default 5)
byte limit = 30;
byte baseLimit = limit;
byte maxCycles = 10;
byte difficulty = 10;
byte difficultyHardening = 5;

// ID of the block used at the moment, never resets, only if it overflows, is just for comparison with the previous block
short blockCounter = 0;

// Point counter (resets at Reset, if points are added when a block manifests it will be sent to Serial Write). Higher levels as well as deleting multiple rows at once will result in getting more points.
short pointCounter = 0;
int pointsThisRound = 0;
int linesThisRound = 0;

int sessionHighscore = 0;

// Game states, when gameOver is activated it will reset at restart, pause whill reset at pressing SELECT (is a bit buggy atm)
boolean gameOver = false;
boolean pause = false;

// The LEDTable to show the game
LEDTable ledtable = LEDTable(LEDTABLE_PIN, 20, 10, PIXELORDER<snake>);

// Initialization of the Controller
Snes controller(clock, strobe, data);

// The array containing the color of the pixel placed in it.
short fieldmap[sizeX][sizeY];

// Array of Colors (RGB mapped to the block type).
static const short colors[7][3] = 
{
  {
    0,255,255
  },
  {
    255,128,0
  },
  {
    255,255,0
  },
  {
    0,0,255
  },
  {
    255,0,0
  },
  {
    0,255,0
  },
  {
    255,0,255
  }
};

// Array of blocks and rotations (7 Blocks with 4 Rotations, each one being created by 4 pixels that have a pair of coordinates related to the relation pixel (mPosX | mPosY)).
static const short blocks[7][4][4][2] = 
{  {  
  { {-1, 0},{ 0, 0},{ 1, 0},{ 2, 0} },   /* cyan    */
  { { 0,-1},{ 0, 0},{ 0, 1},{ 0, 2} },   /*  #*##   */
  { {-1, 0},{ 0, 0},{ 1, 0},{ 2, 0} },   /*         */
  { { 0,-1},{ 0, 0},{ 0, 1},{ 0, 2} }    /*         */
}, {  
  { {-1, 1},{-1, 0},{ 0, 0},{ 1, 0} },   /* orange  */
  { {-1,-1},{ 0,-1},{ 0, 0},{ 0, 1} },   /*  #*#    */
  { {-1, 0},{ 0, 0},{ 1, 0},{ 1,-1} },   /*  #      */
  { { 0,-1},{ 0, 0},{ 0, 1},{ 1, 1} }    /*         */
}, {  
  { { 0, 0},{ 1, 0},{ 0, 1},{ 1, 1} },   /* yellow  */
  { { 0, 0},{ 1, 0},{ 0, 1},{ 1, 1} },   /*   *#    */
  { { 0, 0},{ 1, 0},{ 0, 1},{ 1, 1} },   /*   ##    */
  { { 0, 0},{ 1, 0},{ 0, 1},{ 1, 1} }    /*         */
}, {  
  { {-1, 0},{ 0, 0},{ 1, 0},{ 1, 1} },   /* blue    */
  { { 0,-1},{ 0, 0},{ 0, 1},{-1, 1} },   /*  #*#    */
  { {-1,-1},{-1, 0},{ 0, 0},{ 1, 0} },   /*    #    */
  { { 1,-1},{ 0,-1},{ 0, 0},{ 0, 1} }    /*         */
}, {  
  { {-1, 0},{ 0, 0},{ 0, 1},{ 1, 1} },   /* red     */
  { { 0,-1},{ 0, 0},{-1, 0},{-1, 1} },   /*  #*     */
  { {-1, 0},{ 0, 0},{ 0, 1},{ 1, 1} },   /*   ##    */
  { { 0,-1},{ 0, 0},{-1, 0},{-1, 1} }    /*         */
}, {  
  { { 1, 0},{ 0, 0},{ 0, 1},{-1, 1} },   /* green   */
  { {-1,-1},{-1, 0},{ 0, 0},{ 0, 1} },   /*   *#    */
  { { 1, 0},{ 0, 0},{ 0, 1},{-1, 1} },   /*  ##     */
  { {-1,-1},{-1, 0},{ 0, 0},{ 0, 1} }    /*         */
}, {  
  { {-1, 0},{ 0, 0},{ 1, 0},{ 0, 1} },   /* purple  */
  { { 0,-1},{ 0, 0},{-1, 0},{ 0, 1} },   /*  #*#    */
  { {-1, 0},{ 0, 0},{ 0,-1},{ 1, 0} },   /*   #     */
  { { 0,-1},{ 0, 0},{ 1, 0},{ 0, 1} }    /*         */
} };

void setup()
{
  Serial.begin(9600);

  // Checking if Controller is avaible, if not tetris won't continue but stop.
  if (controller.read() != false) {
    Serial.println("SNES Controller avaible");
    gameOver = true;
  } else {
    Serial.println("SNED Controller unavaible");
  }

  // Initialization of the Fieldmap to -1 (no pixel unavaible).
  initFieldmap();

  // Generate a random Seed from a fluctuating port
  randomSeed(analogRead(0));

  // Activate the LEDTable and set its brightness as well as coloring the game board.
  ledtable.begin();
  ledtable.brightness(40);
  for (int i = 0; i < sizeY; i++) {
    for (int j = 0; j < sizeX; j++) {
      ledtable.fill(j,i,RGB(0,0,64));
    }
  }
  ledtable.show();

  // Get the first random number since it is always 0, forget it and then set the Block and the next block.
  random(7);
  mThisBlock = random(7);
  mNextBlock = random(7);

  // Show the block used at the moment
  showBlock();
}

// Main Loop for the Arduino
void loop()
{
  // Get the controller state
  word state = controller.read();

  // Check again if Controller is connected, otherwise do nothing, then check if the game isn't paused or over
  if (state != false) {
    if (!gameOver && !pause) {
      if (bitRead(state, 4) == false)
      {
        rotate();
        delay(125);
      }
      if (bitRead(state, 5) == false)
      {
        fall();
        delay(20);
      }
      if (bitRead(state, 6) == false)
      {
        left();
        delay(75);
      }
      if (bitRead(state, 7) == false)
      {
        right();
        delay(75);
      }
      if (bitRead(state, 8) == false) {
        down();
        delay(75);
      }
    }
    if (bitRead(state, 2) == false && !gameOver) {
      pause = !pause;
      delay(100);
    }
    if (bitRead(state, 3) == false) {
      reset();
      delay(75);
    }
  }
  counter++;

  // Initiate fall sequence if the limit of frames is reached.
  if (counter >= limit && !gameOver && !pause) {
    fall();
    counter = 0;
  }

  // After a number of blocks manifested (cycles) reduce the limit.
  if (cycles > maxCycles) {
    if (limit > difficulty) {
      limit -= difficultyHardening;
    }
    cycles = 0;
  }

  
  delay(33);
 
}

/* ===========================================
 *  Help Functions
 * ===========================================
 */

// Check if the field is full
boolean fieldFull(int pBlocknumber) {
  return fieldmap[mPosX + blocks[mThisBlock][mRotation][pBlocknumber][0]][mPosY + blocks[mThisBlock][mRotation][pBlocknumber][1]] != -1;
}

// Reset the whole Map
void reset() {
  initFieldmap();
  startNextBlock();
  startNextBlock();
  ledtable.fill(RGB(0,0,64));
  ledtable.show();
  counter = 0;
  Serial.println("Highscore: " + sessionHighscore);
  Serial.println("Your score: " + pointCounter);
  if (pointCounter > sessionHighscore) {
    Serial.println("New Highscore!!! Congratulations!");
    sessionHighscore = pointCounter;
  }
  pointCounter = 0;
  showBlock();
  limit = baseLimit;
  gameOver = false;
}

// Initialize the Field Map to -1
void initFieldmap() {
  for (int x = 0; x < sizeX; x++) {
    for (int y = 0; y < sizeY; y++) {
      fieldmap[x][y] = -1;
    }
  }
}

// Get next Block
void startNextBlock() {
  mPosX = 0;
  mPosY = 5;
  mRotation = 0;
  mThisBlock = mNextBlock;
  mNextBlock = random(7);
  showBlock();
  blockCounter++;
//  showNextBlock();
  if (fieldmap[mPosX][mPosY] != -1) {
    gameOver = true;
  }
}

// Get the X value of the current block with pixel number pPixel
int getX(int pPixel) {
  return mPosX + blocks[mThisBlock][mRotation][pPixel][0];
}

// Get the Y value of the current block with pixel number pPixel
int getY(int pPixel) {
  return mPosY + blocks[mThisBlock][mRotation][pPixel][1];
}

Color getColor() {
  return RGB(colors[mThisBlock][0],colors[mThisBlock][1],colors[mThisBlock][2]);
}

/* 
 * ===========================================
 *  SNES-Controller Commands
 * ===========================================
 */

// A Command
void down() {
  int blockID = blockCounter;

  while (blockID == blockCounter) {
    fall();
  }
}

// DOWN Command
 void fall() {
  clearBlock();
  mPosX++;
  for (int i = 0; i < 4; i++) {
    if (fieldFull(i) || mPosX + blocks[mThisBlock][mRotation][i][0] < 0) {
      mPosX--;
      showBlock();
      manifest();
      startNextBlock();
      return;
    }
  }
  showBlock();
}

// UP Command
void rotate() {
  clearBlock();
  mRotation = (mRotation + 1) % 4;
  for (int i = 0; i < 4; i++) {
    if (fieldFull(i) || mPosY + blocks[mThisBlock][mRotation][i][1] >= sizeY || mPosY + blocks[mThisBlock][mRotation][i][1] < 0 ) {
      mRotation = (mRotation - 1) % 4;
      showBlock();
      return;
    }
  }
  showBlock();
}

// LEFT Command
void left() {
  clearBlock();
  mPosY--;
  for (int i = 0; i < 4; i++) {
    if (fieldFull(i) || mPosY + blocks[mThisBlock][mRotation][i][1] >= sizeY || mPosY + blocks[mThisBlock][mRotation][i][1] < 0) {
      mPosY++;
      showBlock();
      return;
    }
  }
  showBlock();
}

// RIGHT Command
void right() {
  clearBlock();
  mPosY++;
  
  for (int i = 0; i < 4; i++) {
    if (fieldFull(i) || mPosY + blocks[mThisBlock][mRotation][i][1] >= sizeY) {
      mPosY--;
      showBlock();
      return;
    }
  }
  showBlock();
}

/*
 * ==========================
 *  Block Settings
 * ==========================
 */

// Show a not manifested block
void showBlock() {
  for (int i = 0; i < 4; i++) {
    ledtable.fill(getX(i),getY(i),getColor());
  }
  ledtable.show();
}

// Show upcoming block
void showNextBlock() {
  for (int i = 0; i < 4; i++) {
    ledtable.fill(13 + blocks[mThisBlock][mRotation][i][0],3 + blocks[mThisBlock][mRotation][i][1],RGB(colors[mThisBlock][0],colors[mThisBlock][1],colors[mThisBlock][2]));
  }
  
  for (int i = 0; i < 4; i++) {
    ledtable.fill(blocks[13 + mNextBlock][0][i][0],3 + blocks[mNextBlock][0][i][1],RGB(colors[mNextBlock][0],colors[mNextBlock][1],colors[mNextBlock][2]));
  }
  ledtable.show();
}

// clear out the Position of the block, now there will be background color again and the block has to be showed again
boolean clearBlock() {
  for (int i = 0; i < 4; i++) {
    ledtable.fill(getX(i),getY(i),RGB(0,0,64));
  }
}

// Save a Block that reached its final destination in the Field Map
void manifest() {

  for (int i = 0; i < 4; i++) {
    fieldmap[getX(i)][getY(i)] = mThisBlock;
  }
  clearLines();
  cycles++;
}


/*
 * ===========================
 *  Line Clearing
 * ===========================
 */

// Check if there are lines that are to clear, if there are: clear them and show the new score
 void clearLines() {

  boolean lLineClear = true;

  for (int i = sizeX - 1; i >= 0; i--) {
    for (int j = 0; j < sizeY; j++) {
      if (fieldmap[i][j] == -1) {
        lLineClear = false;
        break;
      }
    }
    if (lLineClear) {
      removeLine(i);
      linesThisRound++;
      i++;
    }
    lLineClear = true;
  }

  pointCounter += linesThisRound * pointsThisRound;
  if (linesThisRound * pointsThisRound == 0) {
    Serial.print("New score: ");
    Serial.print(pointCounter);
    Serial.println();
  }
  linesThisRound = 0;
  pointsThisRound = 0;
}

// Remove the line pLine from the LEDTable and the fieldMap
void removeLine(int pLine) {
  
  // Clear the line out of the Field Map
  for (int ledY = pLine; ledY > 0; ledY--) {
    for (int pos = 0; pos < sizeY; pos++) {
      fieldmap[ledY][pos] = fieldmap[ledY-1][pos];
    }
  }

  // Clear the highest line of the Field Map
  for (int i = 0; i < sizeY; i++) {
    fieldmap[i][0] = -1;
  }

  // Set the LED Matrix to the Field Map
  for (int ledY = 0; ledY < sizeX; ledY++) {
    for (int i = 0; i < sizeY; i++) {
      if (fieldmap[ledY][i] != -1) {
        ledtable.fill(ledY,i,RGB(colors[fieldmap[ledY][i]][0],colors[fieldmap[ledY][i]][1],colors[fieldmap[ledY][i]][2]));
      } else {
        ledtable.fill(ledY,i,RGB(0,0,64));
      }
    }
  }
  ledtable.show();
  pointsThisRound += (maxCycles / difficultyHardening) + 1 - (limit / difficultyHardening);
}
