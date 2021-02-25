#ifndef APP_2048GAME_H_
#define APP_2048GAME_H_

bool doNextFrame = false;
int rowSize = 4;
int cellWidth = SCREEN_WIDTH/rowSize;
int cellHeight = APP_SCORE_SCREEN_HEIGHT/rowSize;

int dataArray[rowSize][rowSize]; // [x pos], [y pos] = cellValue
int nextDataArray[rowSize][rowSize]; // [x pos], [y pos] = cellValue
int slideDirection = 0;
int newCellX = 0;
int newCellY = 0;
bool start = false; //will not start without say-so
bool doEndGame = false;
int lastMoveDirection = 0; // to hold the direction moved

// this is the initialization function, for setting up sensors, basically what would be in the setup() func
void app_2048Game_Initialize() {
}

// this is the initial load of the screen, used to add borders and other static pixels to the screen
// calling the reload function is required as its skipped in case its not wanted in a given app
void app_2048Game_load() {
    // draw score top bar separator
    tft->drawFastHLine(0, INFO_BAR_HEIGHT+APP_SCORE_SCREEN_HEIGHT+1, SCREEN_WIDTH, COLOUR_LGA);
    app_2048Game_reload();
}


// this loop is separate to the reload function in case you need other functionality aside from the screen reloading
void app_2048Game_loop() {
}

// for reloading the screen 
void app_2048Game_reload() {
    if (start){
        if (doNextFrame){
            // start by sliding in the direction specified
            switch(slideDirection){
                case 0:{//slide down
                    for (int x=0;x<rowSize;x++){// for each column
                        int nextCellY = 3;
                        bool skipNextY = false;
                        for (int y=3;y>=0;y--){ // for each cell from the bottom
                            if (skipNextY){
                                skipNextY = false;
                            }
                            else if (dataArray[x][y]!= 0){// then its not empty
                                if (y != 0){// then we need to check the above value
                                    if (dataArray[x][y] == dataArray[x][y-1]){// if they are the same, merge them
                                        nextDataArray[x][nextCellY] = dataArray[x][y]*2;
                                        nextCellY=nextCellY-1;
                                        skipNextY=true;
                                    }
                                    else{
                                        nextDataArray[x][nextCellY] = dataArray[x][y];
                                        nextCellY=nextCellY-1;
                                    }
                                }
                            }
                        }
                    }
                }
                case 1:{// slide left
                    for (int y=0;y<rowSize;y++){ // for each row
                        int nextCellX = 0;
                        bool skipNextX = false;
                        for (int x=0;x<rowSize;x++){// for each cell from the left
                            if (skipNextX){
                                skipNextX = false;
                            }
                            else if (dataArray[x][y]!= 0){// then its not empty
                                if (x != 3){// then we need to check the left value
                                    if (dataArray[x][y] == dataArray[x+1][y]){// if they are the same, merge them
                                        nextDataArray[x][nextCellX] = dataArray[x][y]*2;
                                        nextCellX=nextCellX+1;
                                        skipNextX=true;
                                    }
                                    else{
                                        nextDataArray[x][nextCellX] = dataArray[x][y];
                                        nextCellX=nextCellX+1;
                                    }
                                }
                            }
                        }
                    }
                }
                case 2:{// slide up
                    for (int x=0;x<rowSize;x++){// for each column
                        int nextCellY = 0;
                        bool skipNextY = false;
                        for (int y=0;y<rowSize;y++){ // for each cell from the top
                            if (skipNextY){
                                skipNextY = false;
                            }
                            else if (dataArray[x][y]!= 0){// then its not empty
                                if (y != 3){// then we need to check the below value
                                    if (dataArray[x][y] == dataArray[x][y+1]){// if they are the same, merge them
                                        nextDataArray[x][nextCellY] = dataArray[x][y]*2;
                                        nextCellY=nextCellY+1;
                                        skipNextY=true;
                                    }
                                    else{
                                        nextDataArray[x][nextCellY] = dataArray[x][y];
                                        nextCellY=nextCellY+1;
                                    }
                                }
                            }
                        }
                    }
                }
                case 3:{// slide right
                    for (int y=0;y<rowSize;y++){ // for each row
                        int nextCellX = 3;
                        bool skipNextX = false;
                        for (int x=3;x>=0;x--){// for each cell from the right
                            if (skipNextX){
                                skipNextX = false;
                            }
                            else if (dataArray[x][y]!= 0){// then its not empty
                                if (x != 0){// then we need to check the right value
                                    if (dataArray[x][y] == dataArray[x-1][y]){// if they are the same, merge them
                                        nextDataArray[x][nextCellX] = dataArray[x][y]*2;
                                        nextCellX=nextCellX-1;
                                        skipNextX=true;
                                    }
                                    else{
                                        nextDataArray[x][nextCellX] = dataArray[x][y];
                                        nextCellX=nextCellX-1;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // pass new nextDataArray to dataArray and clear nextDataArray for next reload
            for (int x=0;x<rowSize;x++){
                for (int y=0;y<rowSize;y++){
                    dataArray[x][y] = nextDataArray[x][y];
                    nextDataArray[x][y] = 0;
                }
            }

            // add new cell with a value of 1
            bool foundEmptyCell = false;
            while(!foundEmptyCell){
                newCellX = random(0, rowSize-1);
                newCellY = random(0, rowSize-1);
                if (dataArray[newCellX][newCellY] == 0){
                    dataArray[newCellX][newCellY] = 1;
                    foundEmptyCell = true;
                }
            }

            // render cells
            for (int x=0;x<rowSize;x++){
                for (int y=0;y<rowSize;y++){
                    drawCell(x, y);
                }
            }
            lastMoveDirection = slideDirection;
            doNextFrame = false;
        }
    }
    else{// display highscore, last score and start menu
        tft->setTextColor(WHITE); //Start notification
        tft->setTextSize(3);
        tft->setCursor(20, 60);
        tft->print("High Score: ");
        tft->println("--");
        tft->println("");
        tft->print("Last Score: ");
        tft->println("--");
        tft->println("");
        tft->print("| START |");
    }
}

// this is for managing input functionality, use the config INPUT_XXXXX variables in this func
void app_2048Game_input() {
  if (INPUT_UP == 1 && lastMoveDirection !=2){
    slideDirection = 2;
    doNextFrame = true;
    app_2048Game_reload();
  }
  if (INPUT_DOWN == 1 && lastMoveDirection !=0){
    slideDirection = 0;
    doNextFrame = true;
    app_2048Game_reload();
  }
  if (INPUT_LEFT == 1 && lastMoveDirection !=1) {
    slideDirection = 1;
    doNextFrame = true;
    app_2048Game_reload();
  }
  if (INPUT_RIGHT == 1 && lastMoveDirection !=3) {
    slideDirection = 3;
    doNextFrame = true;
    app_2048Game_reload();
  }
  if (INPUT_SELECT == 1 && !start) {
    tft.fillRect(80, 90, 126, 24, BLACK);
    start = true;
    app_2048Game_reload();
  }
  if (INPUT_BACK == 1 || doEndGame == true) {
    tft.fillRect(3, 21, 316, 226, BLACK); //deletes the old game
    app_2048Game_reload();
  }
}


void drawCell(int x, int y){
    int borderTop    =  (y*cellHeight)+1+INFO_BAR_HEIGHT;
    int borderRight  = (x*cellWidth)-1+cellWidth;
    int borderLeft   = (x*cellWidth)+1;
    int borderBottom = (y*cellHeight)-1+INFO_BAR_HEIGHT+cellHeight;
    int cellInnerWidth = borderRight-borderLeft;
    int cellInnerHeight = borderBottom-borderTop;

    tft->fillRect(borderLeft+1, borderTop+1, cellInnerWidth-1, cellInnerHeight-1, COLOUR_MGB);
    //top horizontal line
    tft->drawFastHLine(borderLeft, borderTop, borderRight, COLOUR_LGA);
    // bottom horizontal line
    tft->drawFastHLine(borderLeft, borderBottom, borderRight, COLOUR_LGA);
    //left vertical line
    tft->drawFastVLine(borderLeft, borderTop, borderBottom, COLOUR_LGA);
    // right vertical line
    tft->drawFastHLine(borderRight, borderTop, borderBottom, COLOUR_LGA);

    tft->setTextColor(WHITE); //Start notification
    tft->setTextSize(3);
    tft->setCursor(borderLeft+(cellInnerWidth/5), borderTop+(cellInnerHeight/3));
    if (dataArray[x][y]<10){
        tft->print("  ");
    }
    else if (dataArray[x][y]<100){
        tft->print(" ");
    }
    tft->print(String(dataArray[x][y]))
}

#endif /*  APP_2048GAME_H_ */