#include "app_controller.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "credentials.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

String fetchBitcoinPriceUSD() {
  WiFiClientSecure client;
  client.setInsecure();  // skip certificate check (not safe for production, fine for public APIs)
  HTTPClient http;

  http.begin(client, "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
  http.setTimeout(3000); // 3 seconds timeout

  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    StaticJsonDocument<128> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (!err && doc["bitcoin"]["usd"]) {
      float price = doc["bitcoin"]["usd"];
      char buf[17];
      snprintf(buf, sizeof(buf), "BTC: $%.2f", price);
      http.end();
      return String(buf);
    }
  }

  http.end();
  return "Price error";
}

AppController::AppController(DisplayManager& d, WiFiManagerNB& w, unsigned long interval)
  : display(d), wifi(w), lastState(ConnectionState::Idle),
    lastCheck(0), checkInterval(interval),
    cycleIndex(0), lastCycleTime(0), cycleInterval(9000) {}

void AppController::begin() {
  display.begin();
  wifi.beginConnecting();
  lastCheck = millis();
  lastCycleTime = millis();
  cycleIndex = 0;
}

void AppController::update() {
  unsigned long now = millis();
  ConnectionState state = wifi.getState();

  if (state != lastState) {
    lastState = state;
    display.clear();
    cycleIndex = 0;
    lastCycleTime = now;
    // Show initial state message
    const __FlashStringHelper* msg =
        (state == ConnectionState::Connecting) ? F("Connecting...") :
        (state == ConnectionState::Connected)  ? F("Connected")    :
        (state == ConnectionState::Failed)     ? F("Connect Failed") :
                                                 F("");
    display.print(0, 0, msg);
  }
  if (now - lastPriceFetch > 60000) {  // fetch every 60 seconds
    String price = fetchBitcoinPriceUSD();
    if (!price.startsWith("Price error")) {
      cachedBTCPrice = price;
    }
    lastPriceFetch = now;
  }
  

  if (state == ConnectionState::Idle && (now - lastCheck >= checkInterval)) {
    lastCheck = now;
    if (!wifi.isConnected()) wifi.beginConnecting();
  }


  if (state == ConnectionState::Connected && now - lastCycleTime >= cycleInterval) {
    lastCycleTime = now;
  
    if (cycleIndex == 0) {
      IPAddress ip = WiFi.localIP();
      Serial.println("Step 1 start");
      display.stopScroll();
      display.clearLine(0);

      char buf[17];
      snprintf(buf, sizeof(buf), "%3u.%3u.%3u.%3u", ip[0], ip[1], ip[2], ip[3]);

      scrollBuffer = buf;
      display.print(0, 0, scrollBuffer.c_str());

    } else if (cycleIndex == 1) {
      Serial.println("Step 2 hello");
      uint16_t offset = display.getScrollOffset();
      scrollBuffer += " Well Hello good sir, the current price of Bitcoin is ";
      display.startScroll(0, 0, scrollBuffer.c_str(), 200, offset);

    } else if (cycleIndex == 2) {
      Serial.println("Step 3 BTC");
      uint16_t offset = display.getScrollOffset();
      scrollBuffer = "Well Hello good sir, the current price of Bitcoin is ";
      display.startScroll(0, 0, scrollBuffer.c_str(), 200, offset);
      display.print(1, 0, cachedBTCPrice.c_str());

    } else if (cycleIndex == 3) {
      Serial.println("Step 4 nothingburger");

    } else {
      Serial.println("Step 5 Device Connected");
      uint16_t offset = display.getScrollOffset();
      scrollBuffer += "-----Device Connected-----";
      display.startScroll(0, 0, scrollBuffer.c_str(), 200, offset);

    }
  
    lastCycleIndex = cycleIndex;
    cycleIndex = (cycleIndex + 1) % 5;
  }
  



  display.updateScroll();
}
