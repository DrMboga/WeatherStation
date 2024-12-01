#include "gy21.h"

/*
We have 3 similar sensors which have the same I2C address. That's why we have a muliplexer device.
3 sesors are connected to 3 inputs of multiplexer. Only one sensor can be active at a time.
So, it works like this: Multiplexer is shifted to the input 0, then the first sensor is accessible by its I2C address.
When multiplexer is shifted to 1, second sensor is accessible by the I2C adderss. And so on.
*/

// TCA multiplexer address
#define TCAADDR (0x70)
// Addresses for all GY-21 sensors
#define GY21_ADDRESS (0x40)

// Sends command to reset one sensor
bool resetOneSensor(uint8_t address) {
  uint8_t cmd = 0xFE;
  Wire.beginTransmission(address);

  Wire.write(cmd);

  int error = Wire.endTransmission();

  if (error != 0) {
    return false;
  }

  delay(15);
  return true;
}

// Reads byte of data from one sensor
uint16_t gy21I2cReadData(uint8_t address, uint8_t command) {
  const size_t readLength = 3;
  uint8_t buffer[readLength];

  Wire.beginTransmission(address);
  Wire.write(command);
  int error = Wire.endTransmission();
  if (error != 0) {
    return 0;
  }

  delay(50);

  size_t recv = Wire.requestFrom(address, (uint8_t)readLength);
  if(recv != readLength) {
    return 0;
  }
  for(uint16_t i = 0; i < readLength; i++) {
    buffer[i] = Wire.read();
  }

  uint16_t t = buffer[0];
  t <<= 8;
  t |= buffer[1] & 0b11111100;
  // 3rd byte is the CRC

  return t;
}

// Reads temperature from one sensor
float gy21ReadTemperature(uint8_t address) {
  uint8_t cmd = 0xE3;

  float temperature = gy21I2cReadData(address, cmd);
  temperature *= 175.72f;
  temperature /= 65536.0f;
  temperature -= 46.85f;

  return temperature;
}

// Reads humidity from one sensor
float gy21ReadHumidity(uint8_t address) {
  uint8_t cmd = 0xE5;

  float humidity = gy21I2cReadData(address, cmd);
  humidity *= 125.0f;
  humidity /= 65536.0f;
  humidity -= 6.0f;

  return humidity;
}

// Shifts muliplexer to bus input
void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

bool gy21Setup() {
  Wire.begin();

  // Switching multiplexer to first GY-21 and resetting the sensor
  TCA9548A(0);
  if(!resetOneSensor(GY21_ADDRESS)) {
    return false;
  }

  // Switching multiplexer to second GY-21 and resetting the sensor
  TCA9548A(1);
  if(!resetOneSensor(GY21_ADDRESS)) {
    return false;
  }

  // Switching multiplexer to third GY-21 and resetting the sensor
  TCA9548A(2);
  if(!resetOneSensor(GY21_ADDRESS)) {
    return false;
  }
 
  return true;
}

void readGy21Data(uint8_t sensorIndex, float& temperature, float& humidity) {
    // Switching multiplexer to appropriate sensor
    TCA9548A(sensorIndex);
    temperature = gy21ReadTemperature(GY21_ADDRESS);
    humidity = gy21ReadHumidity(GY21_ADDRESS);
}
