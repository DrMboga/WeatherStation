#include "dht.h"

int pinDHT11 = 15;
SimpleDHT11 dht11(pinDHT11);

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