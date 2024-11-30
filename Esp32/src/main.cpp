#include "dht.h"
#include "lcd.h"

#include <Wire.h>

#define ITERATIONS_FOR_SCREEN_CHANGE (9)
bool isTemperatureShowing = true;
int currentIteration = 0;

// TCA multiplexer
#define TCAADDR (0x70)

// Addresses for all GY-21 sensors
#define GY21_ADDRESS (0x40)


// --- GY-21
void gy21Reset(uint8_t address) {
  uint8_t cmd = 0xFE;
  Wire.beginTransmission(address);

  Wire.write(cmd);

  int error = Wire.endTransmission();

  if (error != 0) {
    Serial.println("Failed to reset!");
  }

  delay(15);
}

uint16_t gy21I2cReadData(uint8_t address, uint8_t command) {
  const size_t readLength = 3;
  uint8_t buffer[readLength];

  Wire.beginTransmission(address);
  Wire.write(command);
  int error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("Failed to set i2c command!");
    return 0;
  }

  delay(50);

  size_t recv = Wire.requestFrom(address, (uint8_t)readLength);
  if(recv != readLength) {
    Serial.println("i2c: Read wrong amount of data");
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

float gy21ReadTemperature(uint8_t address) {
  uint8_t cmd = 0xE3;

  float temperature = gy21I2cReadData(address, cmd);
  temperature *= 175.72f;
  temperature /= 65536.0f;
  temperature -= 46.85f;

  return temperature;
}

float gy21ReadHumidity(uint8_t address) {
  uint8_t cmd = 0xE5;

  float humidity = gy21I2cReadData(address, cmd);
  humidity *= 125.0f;
  humidity /= 65536.0f;
  humidity -= 6.0f;

  return humidity;
}

// --- Multiplexer
void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}
// ---


void setup() { 
  Serial.begin(115200);

  lcdSetup();

  Wire.begin();

  Serial.println("Switching multiplexer to first GY-21 and resetting the sensor");
  TCA9548A(0);
  gy21Reset(GY21_ADDRESS);

  Serial.println("Switching multiplexer to second GY-21 and resetting the sensor");
  TCA9548A(1);
  gy21Reset(GY21_ADDRESS);

  Serial.println("Switching multiplexer to third GY-21 and resetting the sensor");
  TCA9548A(2);
  gy21Reset(GY21_ADDRESS);
}

void loop() {
  float dht11Temperature;
  float dht11Humidity;
  readDht11Data(dht11Temperature, dht11Humidity);

  // Switching multiplexer to first GY-21
  TCA9548A(0);
  float gy211Temperature = gy21ReadTemperature(GY21_ADDRESS);
  float gy211Humidity = gy21ReadHumidity(GY21_ADDRESS);

  // Switching multiplexer to scond GY-21
  TCA9548A(1);
  float gy212Temperature = gy21ReadTemperature(GY21_ADDRESS);
  float gy212Humidity = gy21ReadHumidity(GY21_ADDRESS);

  // Switching multiplexer to third GY-21
  TCA9548A(2);
  float gy213Temperature = gy21ReadTemperature(GY21_ADDRESS);
  float gy213Humidity = gy21ReadHumidity(GY21_ADDRESS);

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