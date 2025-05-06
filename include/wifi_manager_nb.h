#pragma once
#include <ESP8266WiFi.h>

enum class ConnectionState { Idle, Connecting, Connected, Failed };

class WiFiManagerNB {
  const char* ssid;
  const char* pass;
  unsigned long startTime;
  const unsigned long timeout;
  bool connecting;

public:
  WiFiManagerNB(const char* s, const char* p, unsigned long to);
  void beginConnecting();
  ConnectionState getState();
  bool isConnected() const;
};