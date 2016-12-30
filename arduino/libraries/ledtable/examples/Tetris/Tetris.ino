#include <Snes.h>
#include <Adafruit_NeoPixel.h>
#include <ledtable.h>

#define clock 2
#define strobe 3
#define data 4
#define sizeX 20
#define sizeY 10
#define LEDTABLE_PIN 6

byte mThisBlock;
byte mNextBlock;
short mRotation = 0;
short mPosX = 0;
short mPosY = 5;

byte cycles = 0;
byte limit = 30;
short blockCounter = 0;

short pointCounter = 0;
int pointsThisRound = 0;
int linesThisRound = 0;

boolean gameOver = false;
boolean pause = false;

LEDTable ledtable = LEDTable(LEDTABLE_PIN, 20, 10, PIXELORDER<snake>);

Snes controller(clock, strobe, data);
int counter = 0;

short fieldmap[sizeX][sizeY];

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
  Serial.println("SNES");

  initFieldmap();

  randomSeed(analogRead(0));
  
  ledtable.begin();
  ledtable.brightness(20);
  for (int i = 0; i < sizeY; i++) {
    for (int j = 0; j < sizeX; j++) {
      ledtable.fill(j,i,RGB(0,0,64));
    }
  }
  ledtable.show();
  random(7);
  mThisBlock = random(7);
  mNextBlock = random(7);
  showBlock();
}

// Main Loop for the Arduino
void loop()
{
  word state = controller.read();

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
  counter++;

  if (counter >= limit && !gameOver && !pause) {
    fall();
    counter = 0;
  }

  if (cycles > 10) {
    if (limit > 5) {
      limit -= 5;
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
  ledtable.fill(RGB(0,0,64));
  ledtable.show();
  counter = 0;
  pointCounter = 0;
  startNextBlock();
  startNextBlock();
  showBlock();
  limit = 30;
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
  if (fieldmap[mPosX][mPosY] != -1) {
    gameOver = true;
  }
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
    ledtable.fill(mPosX + blocks[mThisBlock][mRotation][i][0],mPosY + blocks[mThisBlock][mRotation][i][1],RGB(colors[mThisBlock][0],colors[mThisBlock][1],colors[mThisBlock][2]));
  }
  ledtable.show();
}

// clear out the Position of the block, now there will be background color again and the block has to be showed again
boolean clearBlock() {
  for (int i = 0; i < 4; i++) {
    ledtable.fill(mPosX + blocks[mThisBlock][mRotation][i][0],mPosY + blocks[mThisBlock][mRotation][i][1],RGB(0,0,64));
  }
}

// Save a Block that reached its final destination in the Field Map
void manifest() {

  for (int i = 0; i < 4; i++) {
    fieldmap[mPosX + blocks[mThisBlock][mRotation][i][0]][mPosY + blocks[mThisBlock][mRotation][i][1]] = mThisBlock;
  }
  clearLines();
  cycles++;
}


/*
 * ===========================
 *  Line Clearing
 * ===========================
 */

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
  //Serial.print("New score: ");
  Serial.print(pointCounter);
  Serial.println();
  linesThisRound = 0;
  pointsThisRound = 0;
}

// Remove a single line
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
  pointsThisRound += 7 - (limit / 5);
}
