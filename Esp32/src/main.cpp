#include "dht.h"
#include "lcd.h"
#include "gy21.h"
#include "secrets.h"

#include <WiFi.h>

/*
Don't forget to add "secrets.h" to the project with this content:
#define WIFI_SSID "your-SSID"
#define WIFI_PASSWORD "your-PASSWORD"
*/

float dht11Temperature = -99.0;
float dht11Humidity = -99.0;

#define ITERATIONS_FOR_SCREEN_CHANGE (7)
bool isTemperatureShowing = true;
int currentIteration = 0;

bool isOffline = true;

void connectToWiFi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  for (size_t i = 0; i < 10; i++)
  {
    delay(500);
    Serial.print(".");
    if (WiFi.status() == WL_CONNECTED) {
      isOffline = false;
      break;
    }
  }
  Serial.println("");
  if (isOffline) {
    Serial.println("WiFi not connected. Going to offline mode.");
  }
  else {
    Serial.println("WiFi connected.");
  }
}

void setup() { 
  Serial.begin(115200);

  lcdSetup();

  if (!gy21Setup()) {
    Serial.println("Unable to setup GY-21 sensors or multiplexer.");
  }

  connectToWiFi();
}

void loop() {
  float currentDht11Temperature;
  float currentDht11Humidity;
  float gy211Temperature;
  float gy211Humidity;
  float gy212Temperature;
  float gy212Humidity;
  float gy213Temperature;
  float gy213Humidity;

  readDht11Data(currentDht11Temperature, currentDht11Humidity);
  // Sometimes dht11 reads data with error. In casee of error, it returns -99.
  // In case of error, showing previous vaules
  if (currentDht11Temperature > -90) {
    dht11Temperature = currentDht11Temperature;
  }
  if (currentDht11Humidity > -90) {
    dht11Humidity = currentDht11Humidity;
  }

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