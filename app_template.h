#ifndef APP_TEMPLATE_H_
#define APP_TEMPLATE_H_








// this is the initialization function, for setting up sensors, basically what would be in the setup() func
void app_template_Initialize() {

}

// this is the initial load of the screen, used to add borders and other static pixels to the screen
// calling the reload function is required as its skipped in case its not wanted in a given app
void app_template_load() {
    // stic pixel drawing here!

    app_template_reload();
}


// this loop is separate to the reload function in case you need other functionality aside from the screen reloading
void app_template_loop() {

}

// for reloading the screen 
void app_template_reload() {

}

// this is for managing input functionality, use the config INPUT_XXXXX variables in this func
void app_template_input() {

}


#endif /*  APP_TEMPLATE_H_ */