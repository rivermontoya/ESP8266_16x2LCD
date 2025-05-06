#pragma once
#include <Arduino.h>
#include "display_manager.h"
#include "wifi_manager_nb.h"

class AppController {
  DisplayManager& display;
  WiFiManagerNB& wifi;
  ConnectionState lastState;
  unsigned long lastCheck;
  const unsigned long checkInterval;

  uint8_t cycleIndex;
  uint8_t lastCycleIndex = 255;  // invalid value at start
  unsigned long lastCycleTime;
  const unsigned long cycleInterval;
  unsigned long lastPriceFetch = 0;
  String cachedBTCPrice = "BTC: ...";
  String scrollBuffer;

public:
  AppController(DisplayManager& d, WiFiManagerNB& w, unsigned long interval = 10000);
  void begin();
  void update();
};