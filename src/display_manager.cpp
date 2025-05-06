#include "display_manager.h"
#include <Arduino.h>
#include <string.h>

DisplayManager::DisplayManager(LiquidCrystal& ref)
  : lcd(ref), scrollStr(nullptr), scrollRow(0), scrollCol(0),
    scrollLen(0), scrollOffset(0), scrollSpeed(0), lastScrollTime(0) {}

void DisplayManager::begin() {
  lcd.begin(16, 2);
}

void DisplayManager::clear() {
  lcd.clear();
}

void DisplayManager::clearLine(uint8_t row) {
  lcd.setCursor(0, row);
  for (uint8_t i = 0; i < 16; ++i) lcd.print(' ');
  lcd.setCursor(0, row);
}

void DisplayManager::print(uint8_t row, uint8_t col, const __FlashStringHelper* str) {
  lcd.setCursor(col, row);
  lcd.print(str);
}

void DisplayManager::print(uint8_t row, uint8_t col, const char* str) {
  lcd.setCursor(col, row);
  lcd.print(str);
}

void DisplayManager::startScroll(uint8_t row, uint8_t col, const char* str, unsigned long speed, uint16_t offset) {
  scrollStr = str;
  scrollRow = row;
  scrollCol = col;
  scrollSpeed = speed;
  scrollLen = strlen(str);
  scrollOffset = offset;
  lastScrollTime = millis();
}

uint16_t DisplayManager::getScrollOffset() const {
  return scrollOffset;
}

void DisplayManager::stopScroll() {
  scrollStr = nullptr;
}

void DisplayManager::updateScroll() {
  if (!scrollStr || scrollLen <= (16 - scrollCol)) return;

  unsigned long now = millis();
  if (now - lastScrollTime < scrollSpeed) return;
  lastScrollTime = now;

  const uint8_t width = 16 - scrollCol;
  char buf[17]; // +1 for null terminator

  for (uint8_t i = 0; i < width; ++i) {
    uint16_t idx = scrollOffset + i;
    buf[i] = (idx < scrollLen) ? scrollStr[idx] : ' ';
  }
  buf[width] = '\0';

  lcd.setCursor(scrollCol, scrollRow);
  lcd.print(buf);

  scrollOffset++;
  if (scrollOffset > scrollLen) {
    scrollOffset = 0;  // Restart only when the text has completely scrolled off
  }
}
