#include "dht.h"
#include "lcd.h"
#include "gy21.h"

#define ITERATIONS_FOR_SCREEN_CHANGE (9)
bool isTemperatureShowing = true;
int currentIteration = 0;

void setup() { 
  Serial.begin(115200);

  lcdSetup();

  if (!gy21Setup()) {
    Serial.println("Unable to setup GY-21 sensors or multiplexer.");
  }
}

void loop() {
  float dht11Temperature;
  float dht11Humidity;
  float gy211Temperature;
  float gy211Humidity;
  float gy212Temperature;
  float gy212Humidity;
  float gy213Temperature;
  float gy213Humidity;

  readDht11Data(dht11Temperature, dht11Humidity);

  readGy21Data(0, gy211Temperature, gy211Humidity);
  readGy21Data(1, gy212Temperature, gy212Humidity);
  readGy21Data(2, gy213Temperature, gy213Humidity);

  // Show result
  currentIteration++;
  if (ITERATIONS_FOR_SCREEN_CHANGE == currentIteration) {
    currentIteration = 0;
    isTemperatureShowing = !isTemperatureShowing;
    lcdClear();
  }


  if (isTemperatureShowing) {
    lcdShowTemeperature(
      dht11Temperature,
      gy211Temperature,
      gy212Temperature,
      gy213Temperature
    );
  }
  else {
    lcdShowHumidity(
      dht11Humidity,
      gy211Humidity,
      gy212Humidity,
      gy213Humidity
    );
  }
  
  Serial.print("DHT11 temperature: ");
  Serial.print((int)dht11Temperature);
  Serial.print("°, ");
  Serial.print((int)dht11Humidity);
  Serial.print("% | GY-21 No 1: ");
  Serial.print((int)gy211Temperature);
  Serial.print("°, ");
  Serial.print((int)gy211Humidity);
  Serial.print("% | GY-21 No 2: ");
  Serial.print((int)gy212Temperature);
  Serial.print("°, ");
  Serial.print((int)gy212Humidity);
  Serial.print("% | GY-21 No 3: ");
  Serial.print((int)gy213Temperature);
  Serial.print("°, ");
  Serial.print((int)gy213Humidity);
  Serial.println("%");

  delay(1500); // DHT11 sampling rate is 1HZ.
}