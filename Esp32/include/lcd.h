#include <LiquidCrystal.h>
#include <Arduino.h>

void lcdSetup();
int lcdCalculateStartColumn(int value);
void lcdClear();
void lcdShowTemeperature(float dht11Temperature, float gy211Temperature, float gy212Temperature, float gy213Temperature);
void lcdShowHumidity(float dht11Humidity, float gy211Humidity, float gy212Humidity, float gy213Humidity);
void showForecastWord(const String& word);