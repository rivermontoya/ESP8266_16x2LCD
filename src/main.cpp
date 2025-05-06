#include <Arduino.h>
#include <LiquidCrystal.h>
#include "display_manager.h"
#include "wifi_manager_nb.h"
#include "app_controller.h"
#include "credentials.h"

LiquidCrystal lcd(D2, D1, D6, D7, D5, D4);
DisplayManager display(lcd);
WiFiManagerNB wifiManager(WIFI_SSID, WIFI_PASS, 15000);
AppController app(display, wifiManager, 10000);

void setup() {
  Serial.begin(921600);
  app.begin();
}

void loop() {
  app.update();
}