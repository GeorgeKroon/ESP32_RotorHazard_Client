#include <Arduino.h>
#include "WiFi.h"
#include "Wire.h"
#include "config/config.h"
#include "config/enums.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "tasks/updateDisplay.h"
#include "tasks/wifi-connection.h"
#include "tasks/wifi-update-signalstrength.h"
#include "tasks/fetch-time-from-ntp.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DisplayValues gDisplayValues;

void setup() {
  #if DEBUG == true
    Serial.begin(115200);
  #endif
  Wire.begin(5,4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)){
    Serial.println("SSD1306 allocation failed");
    delay(10*1000);
    ESP.restart();
  }

  display.clearDisplay();
  display.setRotation(3);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  // ----------------------------------------------------------------
  // TASK: Update the display every second
  //       This is pinned to the same core as Arduino
  //       because it would otherwise corrupt the OLED
  // ----------------------------------------------------------------
  xTaskCreatePinnedToCore(
    updateDisplay,
    "UpdateDisplay",  // Task name
    10000,            // Stack size (bytes)
    NULL,             // Parameter
    3,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  /**
   * TASK: Connect to WiFi & keep the connection alive
   */
  xTaskCreatePinnedToCore(
    keepWiFiAlive,
    "keepWiFiAlive",  // Task name
    5000,            // Stack size (bytes)
    NULL,             // Parameter
    1,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );

  // ----------------------------------------------------------------
  // TASK: update WiFi signal strength
  // ----------------------------------------------------------------
  xTaskCreate(
    updateWiFiSignalStrength,
    "Update WiFi strength",
    1000,             // Stack size (bytes)
    NULL,             // Parameter
    2,                // Task priority
    NULL              // Task handle
  );

  // ----------------------------------------------------------------
  // TASK: update time from NTP server.
  // ----------------------------------------------------------------
  #if NTP_TIME_SYNC_ENABLED == true
    xTaskCreate(
      fetchTimeFromNTP,
      "Update NTP time",
      5000,            // Stack size (bytes)
      NULL,             // Parameter
      1,                // Task priority
      NULL              // Task handle
    );
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}