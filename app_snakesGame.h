#ifndef APP_SNAKESGAME_H_
#define APP_SNAKESGAME_H_

/*
  TO DO:
    setup the food to only appear where there is not a snake
    fix all text printing to display in the correct scoring bar!!!
    add HighScore Show function
    add Save High Score option on death
    add Name Set Functionality
    add blink function to start and retry text

  FUTURE FEATURES:
    add snake circling start high score text on start screen
    add levels??? 
        based on speed of snake?
        or add mines that have a timeout on them?
        or add timeouts on the food?

*/

#define SNAKE_COLOUR 0xE549
#define FOOD_COLOUR 0x5F09

// get the max snake array size
int gridWidth = SCREEN_WIDTH/4;
int gridHeight = APP_SCORE_SCREEN_HEIGHT/4;
int gridPixelSize = gridWidth*gridHeight;

bool start = false; //will not start without say-so
bool doEndGame = false;
int headX = 4;        //coordinates for head
int headY = 4;
int tailLength = 0;
int tailCoords[gridPixelSize][2];
int changeX = 0;      //the direction of the snake
int changeY = 4;
int lastMoveDirection = 0; // to hold the direction moved
int score = 1;
int foodX;            //coordinates of food
int foodY;
bool eaten = true; //if true a new food will be made
int loopCount = 0; //number of times the loop has run
int clearPoint = 0;  //when the loopCount is reset
bool clearScore = false;

void app_snakesGame_Initialize() {
}


void app_snakesGame_load() {
  // draw top border
  tft->drawFastHLine(0, INFO_BAR_HEIGHT, SCREEN_WIDTH, COLOUR_LGA);
  // draw bottom border
  tft->drawFastHLine(0, INFO_BAR_HEIGHT+gridHeight+1, SCREEN_WIDTH, COLOUR_LGA);
  app_snakesGame_reload();
}

void app_snakesGame_loop() {
}

void app_snakesGame_reload() {
  if (clearScore and start) { //resets score from last game, won't clear
    tft->setTextColor(WHITE); //Start notification
    tft->setTextSize(3);
    tft->setCursor(80, 90);
    tft->print("START");

    tft->setTextColor(BLACK); //Score keeper
    tft->setTextSize(2);
    tft->setCursor(5, 3);
    tft->print("Score: ");
    printScore();

    randomSeed(analogRead(6)); //make every game unique

    score = 1;                //until new game starts so you can show off
    printScore();             //your own score
    clearScore = false;
    for (int x=0;x<gridPixelSize;x++){
      tailCoords[x][0] = 0;
      tailCoords[x][1] = 0;
    }
  }
  if (start) {
    // roll back array of tail coordinates
    for (int x=1;x<gridPixelSize;x++){
      tailCoords[x-1][0] = tailCoords[x][0];
      tailCoords[x-1][1] = tailCoords[x][1];
    }
    tailCoords[gridPixelSize][0] = headX;  //adds current head coordinates to be
    tailCoords[gridPixelSize][1] = headY;  //covered later
    headX = headX + (changeX);  //head moved
    headY = headY + (changeY); 
    
    if (headX == foodX && headY == foodY) { //food
      score += 1 + (score/50);// exponentially increase the score for INTENSE scoring
      printScore();
      eaten = true;
      tailLength++;
    }

    if (tailLength > gridPixelSize-3) {
      score = score*10;// you filled the screen! max score for you!!
      win();
    }
    drawSnake(headX, headY);
 
    // create new food if its been eaten and make sure its not on the snake
    if (eaten) {
      bool foundSpace = false;
      while (!foundSpace){
        bool spaceTaken = false;
        foodX = random(2, SCREEN_WIDTH/2);
        foodY = random(2, APP_SCREEN_HEIGHT/2);
        for (int z = tailLength;z<gridPixelSize; z++){
          if (foodX == tailCoords[z][0] && foodY == tailCoords[z][1]){
            spaceTaken = true;
          }
        }
        if (!spaceTaken){
          foundSpace = true;
        }
      }
      eaten = false;
    }
    drawDotRed(foodX, foodY); //draw the food

    //Death By Boarder!
    if (headX > gridWidth or headX < 1 or headY < 1 or headY > gridHeight) { 
      doEndGame = true;
    }

    //Death By eating TAIL!
    for (int x=(gridPixelSize-tailLength);x<gridPixelSize;x++){
      if (tailCoords[gridPixelSize][0] == headX && tailCoords[gridPixelSize][1] == headY){
        doEndGame = true;
      } 
    }
  }
}

void app_snakesGame_input() {
  if (INPUT_UP==0 && INPUT_DOWN==0 && INPUT_LEFT==0 && INPUT_RIGHT==0){
    switch(lastMoveDirection){
      case 0:{// down
        changeX = 0;
        changeY = 4;
      }
      case 1:{// left
        changeX = -4;
        changeY = 0;
      }
      case 2:{// up
        changeX = 0;   //changes the direction of the snake
        changeY = -4;
      }
      case 3:{// right
        changeX = 4;
        changeY = 0;
      }      
    }
  }

  if (INPUT_UP == 1 && lastMoveDirection !=0){
    changeX = 0;   //changes the direction of the snake
    changeY = -4;
  }

  if (INPUT_DOWN == 1 && lastMoveDirection !=2){
    changeX = 0;
    changeY = 4;
  }

  if (INPUT_LEFT == 1 && lastMoveDirection !=3) {
    changeX = -4;
    changeY = 0;
  }

  if (INPUT_RIGHT == 1 && lastMoveDirection !=1) {
    changeX = 4;
    changeY = 0;
  }

  if (INPUT_SELECT == 1 && !start) {
    tft.fillRect(80, 90, 126, 24, BLACK);
    start = true;
  }
  if (INPUT_BACK == 1 || doEndGame == true) {
    tft.fillRect(3, 21, 316, 226, BLACK); //deletes the old game
    
    eaten = true; //new food will be created
    
    tft->setCursor(80, 90);       //Retry message
    tft->setTextSize(3);
    tft->setTextColor(WHITE);
    tft->print("RETRY?");
    tft->setTextColor(BLACK); //sets back to scoreboard settings
    tft->setTextSize(2);
    tft->setCursor(5, 3);
    tft->print("Length: ");
    headX = 4;              //reset snake
    headY = 4;
    changeX = 0;
    changeY = 4;

    //memset(tailCoordsX, 0, gridPixelSize); //clear the tailCoords arrays
    //memset(tailCoordsY, 0, gridPixelSize); //probably not necessary

    loopCount = 0;
    clearScore = true;
    start = false;     //stops game
    doEndGame = false;
  }
}

void drawSnake(int x, int y) {
  tft->fillRect(
    x-2,                  //left most X pos
    y-2+INFO_BAR_HEIGHT,  //top most Y pos (info bar offset)
    4,                    //fill width
    4,                    //fill height
    SNAKE_COLOUR);       //colour
}

void drawFood(int x, int y) {
  tft->fillRect(
    x-2, 
    y-2+INFO_BAR_HEIGHT, 
    4, 
    4, 
    FOOD_COLOUR);
}

void shiftTail(int x, int y) {
  tft->fillRect(
    x-2, 
    y-2+INFO_BAR_HEIGHT, 
    4, 
    4, 
    BLACK);
}

void displayScore() {
  tft->fillRect(88, 3, 50, 16, BLACK);//clears old score
  tft->setCursor(88, 3);
  tft->print(score);                          //prints current score
}

void win() {
    tft.fillRect(3, 21, 316, 226, BLACK); //deletes the old game
    tft->setCursor(80, 90);       //Retry message
    tft->setTextSize(3);
    tft->setTextColor(WHITE);
    tft->print("YOU WIN!!");
    tft->setTextColor(BLACK); //sets back to scoreboard settings
    tft->setTextSize(2);
    tft->setCursor(5, 3);
    tft->print("Length: ");
    headX = 4;              //reset snake
    headY = 4;
    changeX = 0;
    changeY = 4;
    eaten = true; //new food will be created
    //memset(tailCoordsX, 0, gridPixelSize); //clear the tailCoords arrays
    //memset(tailCoordsY, 0, gridPixelSize); //probably not necessary
    loopCount = 0;
    clearScore = true;
    start = false;     //stops game
    doEndGame = false;
}

#endif /*  APP_SNAKESGAME_H_ */