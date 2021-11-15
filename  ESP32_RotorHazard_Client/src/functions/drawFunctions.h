#ifndef DRAW_FUNCTIONS
#define DRAW_FUNCTIONS

#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include "../config/enums.h"
#include "../config/config.h"

extern Adafruit_SSD1306 display;
extern DisplayValues gDisplayValues;

/**
 * The screen that is displayed when the ESP has just booted
 * and is connecting to WiFi & AWS.
 */
void drawBootscreen(){
  byte X = 14;
  byte Y = 70;
  byte WIDTH = 6;
  byte MAX_HEIGHT = 35;
  byte HEIGHT_STEP = 10;
  byte X_SPACING = 10;

  display.fillRect(X              , Y, WIDTH, MAX_HEIGHT - HEIGHT_STEP*3, WHITE);
  display.fillRect(X + X_SPACING  , Y - HEIGHT_STEP, WIDTH, MAX_HEIGHT - HEIGHT_STEP*2, WHITE);
  display.fillRect(X + X_SPACING*2, Y - HEIGHT_STEP*2, WIDTH, MAX_HEIGHT - HEIGHT_STEP, WHITE);
  display.fillRect(X + X_SPACING*3, Y - HEIGHT_STEP*3, WIDTH, MAX_HEIGHT, WHITE);

  display.setTextSize(1);
  display.setCursor(0, Y + MAX_HEIGHT / 2);
  display.println("Connecting");

  if(gDisplayValues.currentState == CONNECTING_WIFI){
    display.println("   WiFi");
  }
}

void drawTime(){
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(gDisplayValues.time);
}

void drawSignalStrength(){
  const byte X = 51;
  const byte X_SPACING = 2;

  // Draw the four base rectangles
  display.fillRect(X, 8-2, 1, 2, WHITE); // Bar 1
  display.fillRect(X + X_SPACING, 8-2, 1, 2, WHITE); // Bar 2
  display.fillRect(X + X_SPACING*2, 8-2, 1, 2, WHITE); // Bar 3
  display.fillRect(X + X_SPACING*3, 8-2, 1, 2, WHITE); // Bar 4

  // Draw bar 2
  if(gDisplayValues.wifi_strength > -70){
    display.fillRect(X+X_SPACING, 8-4, 1, 4, WHITE);
  }

  // Draw bar 3
  if(gDisplayValues.wifi_strength > -60){
    display.fillRect(X+X_SPACING*2, 8-6, 1, 6, WHITE);
  }

  // Draw bar 4
  if(gDisplayValues.wifi_strength >= -50){
    display.fillRect(X+X_SPACING*3, 8-8, 1, 8, WHITE);
  }
}

#endif