#include "lcd.h"

const uint8_t rs = 26;
const uint8_t en = 25;
const uint8_t d4 = 33;
const uint8_t d5 = 32;
const uint8_t d6 = 18;
const uint8_t d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const uint8_t weatherStartSymbolIndex = 5;
const uint8_t weatherSymbolsCount = 11;

void lcdSetup() {
  lcd.begin(16,2);
  lcd.clear();
}

int lcdCalculateStartColumn(int value) {
  // 2 digit value or one digit with minus sign
  if ((value >= 10 && value < 100)  ||
    (value < 0 && value > -10)) {
    return 1;
  }
  // One digit value
  if ((value >= 0 && value < 10)) {
    return 2;
  }
  // 3 digit value or 2 digit with minus sign
  return 0;
}

void lcdClear() {
    lcd.clear();
}

void setBorders() {
    lcd.setCursor(0, 0);
    lcd.print("    |");
    lcd.setCursor(4, 1);
    lcd.print("|");
    lcd.setCursor(11, 1);
    lcd.print("|");
}

void lcdShowTemeperature(float dht11Temperature, float gy211Temperature, float gy212Temperature, float gy213Temperature) {
    setBorders();
    lcd.setCursor(lcdCalculateStartColumn((int)dht11Temperature), 0);
    lcd.print((int)dht11Temperature);
    lcd.setCursor(3, 0);
    lcd.print((char)223); // Celsius symbol

    lcd.setCursor(lcdCalculateStartColumn((int)gy211Temperature), 1);
    lcd.print((int)gy211Temperature);
    lcd.setCursor(3, 1);
    lcd.print((char)223); // Celsius symbol

    lcd.setCursor(6 + lcdCalculateStartColumn((int)gy212Temperature), 1);
    lcd.print((int)gy212Temperature);
    lcd.setCursor(9, 1);
    lcd.print((char)223); // Celsius symbol

    lcd.setCursor(12 + lcdCalculateStartColumn((int)gy213Temperature), 1);
    lcd.print((int)gy213Temperature);
    lcd.setCursor(15, 1);
    lcd.print((char)223); // Celsius symbol
}

void lcdShowHumidity(float dht11Humidity, float gy211Humidity, float gy212Humidity, float gy213Humidity) {
    setBorders();
    lcd.setCursor(lcdCalculateStartColumn((int)dht11Humidity), 0);
    lcd.print((int)dht11Humidity);
    lcd.setCursor(3, 0);
    lcd.print("%");

    lcd.setCursor(lcdCalculateStartColumn((int)gy211Humidity), 1);
    lcd.print((int)gy211Humidity);
    lcd.setCursor(3, 1);
    lcd.print("%");

    lcd.setCursor(6 + lcdCalculateStartColumn((int)gy212Humidity), 1);
    lcd.print((int)gy212Humidity);
    lcd.setCursor(9, 1);
    lcd.print("%");

    lcd.setCursor(12 + lcdCalculateStartColumn((int)gy213Humidity), 1);
    lcd.print((int)gy213Humidity);
    lcd.setCursor(15, 1);
    lcd.print("%");
}

void showForecastWord(const String& word) {
  //weatherStartSymbolIndex
  int leadingSpaces = weatherSymbolsCount - word.length();
  for (size_t i = 0; i < leadingSpaces; i++)
  {
    lcd.setCursor(i + weatherStartSymbolIndex, 0);
    lcd.print(" ");
  }
  lcd.setCursor(leadingSpaces + weatherStartSymbolIndex, 0);
  lcd.print(word);
}