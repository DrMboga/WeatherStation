#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define ITERATIONS_FOR_SCREEN_CHANGE (9)
bool isTemperatureShowing = true;
int currentIteration = 0;

// DHT11
int pinDHT11 = 15;
SimpleDHT11 dht11(pinDHT11);

// TCA multiplexer
#define TCAADDR (0x70)

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

// --- Multiplexer
void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}
// ---

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
    lcd.clear();
  }

  lcd.setCursor(4, 0);
  lcd.print("|");
  lcd.setCursor(4, 1);
  lcd.print("|");
  lcd.setCursor(11, 1);
  lcd.print("|");
  if (isTemperatureShowing) {
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
  else {
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