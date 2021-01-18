#ifndef CONFIG_H_
#define CONFIG_H_






// ---------------------------------- //
// Manually Configure Below variables //
// ---------------------------------- //

// the number of app you have
const int APP_COUNT = 5; // increment this if you add an app

// these are arrays of the 4 main functions that all apps should have
int appInitializeFunctions[APP_COUNT]; 
int appFirstLoadFunctions[APP_COUNT]; 
int appReloadFunctions[APP_COUNT]; 
int appInputCheckFunctions[APP_COUNT]; 
int appLoopFunctions[APP_COUNT]; 
char appName[APP_COUNT][20]; 
int appLoopDelayMS[APP_COUNT];   // SET to 0 if not used!
int appReloadDelayMS[APP_COUNT]; 

// screen width and height, set this to what your screen uses
const int SCREEN_WIDTH = 176;
const int SCREEN_HEIGHT = 220;
const int INFO_BAR_HEIGHT = 21;
const int APP_SCREEN_HEIGHT = SCREEN_HEIGHT - INFO_BAR_HEIGHT;

// add a variable here to use in apps and set the variable to the next available index
const int APP_THERMAL_ID = 0;
const int APP_SNAKE_ID = 1;

// input setup, set these variables in your input function if a button is pressed/touched or not (or add more)
int INPUT_LEFT = 0;
int INPUT_RIGHT = 0;
int INPUT_UP = 0;
int INPUT_DOWN = 0;
int INPUT_BACK = 0;
int INPUT_SELECT = 0;

// setup links to all functions and app variables here!
void appsInitialize(){
    // setup thermal app
    appName[APP_THERMAL_ID] =                "Thermal Camera App";
    appReloadDelayMS[APP_THERMAL_ID] =       33;
    appLoopDelayMS[APP_THERMAL_ID] =         0;
    appInitializeFunctions[APP_THERMAL_ID] = app_thermalCamera_Initialize;
    appFirstLoadFunctions[APP_THERMAL_ID] =  app_thermalCamera_load;
    appReloadFunctions[APP_THERMAL_ID] =     app_thermalCamera_reload;
    appLoopFunctions[APP_THERMAL_ID] =       app_thermalCamera_loop;
    appInputCheckFunctions[APP_THERMAL_ID] = app_thermalCamera_input;

    appName[APP_SNAKE_ID] =                  "Snake Game App";
    appReloadDelayMS[APP_SNAKE_ID] =         180;
    appLoopDelayMS[APP_SNAKE_ID] =           0;
    appInitializeFunctions[APP_SNAKE_ID] =   app_thermalCamera_Initialize;
    appFirstLoadFunctions[APP_SNAKE_ID] =    app_thermalCamera_load;
    appReloadFunctions[APP_SNAKE_ID] =       app_thermalCamera_reload;
    appLoopFunctions[APP_SNAKE_ID] =         app_thermalCamera_loop;
    appInputCheckFunctions[APP_SNAKE_ID] =   app_thermalCamera_input;

}



// ----------------------------- //
// Automatic Configuration Below //
// ----------------------------- //


if (SCREEN_WIDTH>SCREEN_HEIGHT) {
    const int SCREEN_MODE = 0;//Landscape mode
}
else {
    const int SCREEN_MODE = 1;//Portrait mode
}

#endif /*  CONFIG_H_ */