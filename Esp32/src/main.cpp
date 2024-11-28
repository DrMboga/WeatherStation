#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal.h>

int pinDHT11 = 15;
SimpleDHT11 dht11(pinDHT11);

// Addresses for all GY-21 sensors
#define GY21_ADDRESS (0x40)

// LCD
const uint8_t rs = 26;
const uint8_t en = 25;
const uint8_t d4 = 33;
const uint8_t d5 = 32;
const uint8_t d6 = 18;
const uint8_t d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcdSetup() {
  lcd.begin(16,2);
  lcd.clear();
}


void readDht11Data(float& temperature, float& humidity) {
  temperature = 0;
  humidity = 0;
  int err = SimpleDHTErrSuccess;
  if((err=dht11.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess){
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(2000);
    return;
  }
}

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

// ---

void setup() { 
  Serial.begin(115200);

  lcdSetup();

  Wire.begin();

  gy21Reset(GY21_ADDRESS);
}

void loop() {
  float dht11Temperature;
  float dht11Humidity;
  readDht11Data(dht11Temperature, dht11Humidity);

  float gy211Temperature = gy21ReadTemperature(GY21_ADDRESS);
  float gy211Humidity = gy21ReadHumidity(GY21_ADDRESS);

  lcd.setCursor(0, 0);
  lcd.print("DHT11:");
  lcd.setCursor(7, 0);
  lcd.print((int)dht11Temperature);
  lcd.setCursor(9, 0);
  lcd.print((char)223); // Celsius symbol
  lcd.setCursor(10, 0);
  lcd.print(",");
  lcd.setCursor(12, 0);
  lcd.print((int)dht11Humidity);
  lcd.setCursor(14, 0);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("GY-21:");
  lcd.setCursor(7, 1);
  lcd.print((int)gy211Temperature);
  lcd.setCursor(9, 1);
  lcd.print((char)223); // Celsius symbol
  lcd.setCursor(10, 1);
  lcd.print(",");
  lcd.setCursor(12, 1);
  lcd.print((int)gy211Humidity);
  lcd.setCursor(14, 1);
  lcd.print("%");

  Serial.print("DHT11 temperature: ");
  Serial.print((int)dht11Temperature);
  Serial.print("°, ");
  Serial.print((int)dht11Humidity);
  Serial.print("% | GY-21 No 1: ");
  Serial.print((int)gy211Temperature);
  Serial.print("°, ");
  Serial.print((int)gy211Humidity);
  Serial.println("%");

  delay(1500); // DHT11 sampling rate is 1HZ.
}