#include "wifi_manager_nb.h"
#include <Arduino.h>

WiFiManagerNB::WiFiManagerNB(const char* s, const char* p, unsigned long to)
  : ssid(s), pass(p), startTime(0), timeout(to), connecting(false) {}

void WiFiManagerNB::beginConnecting() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.begin(ssid, pass);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  startTime = millis();
  connecting = true;
}

ConnectionState WiFiManagerNB::getState() {
  if (connecting) {
    if (WiFi.status() == WL_CONNECTED) {
      connecting = false;
      return ConnectionState::Connected;
    }
    if (millis() - startTime >= timeout) {
      connecting = false;
      return ConnectionState::Failed;
    }
    return ConnectionState::Connecting;
  }
  return (WiFi.status() == WL_CONNECTED)
      ? ConnectionState::Connected
      : ConnectionState::Idle;
}

bool WiFiManagerNB::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}