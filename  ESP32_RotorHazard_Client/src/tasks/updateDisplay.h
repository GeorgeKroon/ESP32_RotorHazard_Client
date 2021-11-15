#ifndef TASK_UPDATE_DISPLAY
#define TASK_UPDATE_DISPLAY

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "../functions/drawFunctions.h"
#include "../config/config.h"

extern Adafruit_SSD1306 display;
extern DisplayValues gDisplayValues;

/**
 * Metafunction that takes care of drawing all the different
 * parts of the display (or not if it's turned off).
 */
void updateDisplay(void * parameter){
  for (;;){
    //serial_println(F("[LCD] Updating..."));
    display.clearDisplay();

    if(gDisplayValues.currentState == CONNECTING_WIFI){
      drawBootscreen();
    }
    
    if(gDisplayValues.currentState == UP || 
        gDisplayValues.currentState == STARTINGPROCEDURE){
      
      drawTime();
      drawSignalStrength();
    }

    display.display();

    // Sleep for .5 seconds, then update display again!
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

#endif