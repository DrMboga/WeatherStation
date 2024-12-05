#include <Arduino.h>

String getForecastWord();

bool sendReadingsToBackend(
    time_t epoch,
    float outTemp,
    float outHum,
    float room1Temp,
    float room1Hum,
    float room2Temp,
    float room2Hum,
    float room3Temp,
    float room3Hum
);