#include "backendClient.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* hourlyForecastUrl = "http://192.168.0.65:8830/getweatherforecastnexthour";

String getWordByIcon(int icon);

String getForecastWord() {
    String response = "Undefined";

    HTTPClient http;
    http.begin(hourlyForecastUrl);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
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
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        response = "HTTP Err";
      }

    // Free resources
    http.end();
    return response;
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