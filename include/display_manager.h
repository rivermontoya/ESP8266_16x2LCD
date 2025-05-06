#pragma once
#include <LiquidCrystal.h>
#include <Arduino.h>

class DisplayManager {
  LiquidCrystal& lcd;
  const char* scrollStr;
  uint8_t scrollRow, scrollCol;
  uint16_t scrollLen, scrollOffset;
  unsigned long scrollSpeed, lastScrollTime;

public:
  DisplayManager(LiquidCrystal& ref);
  void begin();
  void clear();
  void clearLine(uint8_t row);
  void print(uint8_t row, uint8_t col, const __FlashStringHelper* str);
  void print(uint8_t row, uint8_t col, const char* str);
  void startScroll(uint8_t row, uint8_t col, const char* str, unsigned long speed, uint16_t offset = 0);
  void updateScroll();
  void stopScroll();
  uint16_t getScrollOffset() const;
};