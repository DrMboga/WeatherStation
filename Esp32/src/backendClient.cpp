#include "backendClient.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* hourlyForecastUrl = "http://192.168.0.65:8830/getweatherforecastnexthour";
const char* sendReadingsUrl = "http://192.168.0.65:8830/savesensorreadings";

String getWordByIcon(int icon);

String getForecastWord() {
    String response = "Undefined";

    HTTPClient http;
    http.begin(hourlyForecastUrl);

    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        
        JSONVar forecast = JSON.parse(payload);
        if (JSON.typeof(forecast) == "undefined") {
            Serial.println("Parsing input failed!");
            response = "No JSON";
        }
        else {
            /*
            "weatherIcon" = 12
            "iconPhrase" = "Showers"            
            */
           int icon = forecast["weatherIcon"];
           String phrase = forecast["iconPhrase"];
           Serial.print(icon);
           Serial.print(": ");
           Serial.println(phrase);
           response = getWordByIcon(icon);
        }
      }
      else if (httpResponseCode == 503) {
        Serial.println("AccuWeather limit achieved");
        response = "Limit exc.";
      }  
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        response = "HTTP Err";
      }

    // Free resources
    http.end();
    return response;
}

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
) {
    Serial.println("Send readings");
    HTTPClient http;
    http.begin(sendReadingsUrl);

    http.addHeader("Content-Type", "application/json");
    String httpRequestData = "{\"measurementTimeStamp\": ";
    httpRequestData += String(epoch) + ",\"outside\": {\"temperature\":";
    httpRequestData += String(outTemp) + ",\"humidity\": ";
    httpRequestData += String(outHum) + "}, \"room1\": { \"temperature\": ";
    httpRequestData += String(room1Temp) + ", \"humidity\": ";
    httpRequestData += String(room1Hum) + " }, \"room2\": { \"temperature\": ";
    httpRequestData += String(room2Temp) + ", \"humidity\": ";
    httpRequestData += String(room2Hum) + " }, \"room3\": { \"temperature\": ";
    httpRequestData += String(room3Temp) + ", \"humidity\": ";
    httpRequestData += String(room3Hum) + " }}";

    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("Response code: ");
    Serial.println(httpResponseCode);

    bool result = httpResponseCode > 0;

    // Free resources
    http.end();
    return result;
}


String getWordByIcon(int icon) {
    // Codes are taken from here https://developer.accuweather.com/weather-icons
    switch (icon)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return "Sunny";
    case 6:
    case 7:
    case 36:
    case 38:
        return "Cloudy";
    case 8:
        return "Overcast";
    case 11:
    case 37:
        return "Fog";
    case 12:
    case 13:
    case 14:
    case 39:
    case 40:
        return "Showers";
    case 15:
    case 16:
    case 17:
    case 41:
    case 42:
        return "T-Storms";
    case 18:
        return "Rain";
    case 19:
    case 20:
    case 21:
    case 43:
        return "Flurries";
    case 22:
    case 23:
    case 44:
        return "Snow";
    case 24:
        return "Ice";
    case 25:
    case 26:
    case 29:
        return "Rain + Snow";
    case 30:
        return "Hot";
    case 31:
        return "Cold";
    case 32:
        return "Windy";
    case 33:
    case 34:
        return "Clear";
    default:
        return "No info";
    }
}