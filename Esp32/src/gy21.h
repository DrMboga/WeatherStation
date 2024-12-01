#include <Wire.h>

bool gy21Setup();

void readGy21Data(uint8_t sensorIndex, float& temperature, float& humidity);