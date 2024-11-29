using System;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Services;

public interface IForecast
{
    /// <summary>
    /// Returns the location info by postal code. The <href>AccuWeatherLocationResponse.Key</href> should be stored in the app settings for future forecast requests.
    /// </summary>
    Task<AccuWeatherLocationResponse[]> GetLocationByPostalCode(string postalCode);

    /// <summary>
    /// Returns forecast for next hour. The location key should be in the app settings
    /// </summary>
    Task<AccuWeatherHourlyForecastResponse> GetNextHourForecast();

    /// <summary>
    /// Returns forecast for next 12 hours. The location key should be in the app settings.
    /// </summary>
    Task<AccuWeatherHourlyForecastResponse[]> GetNextTwelveHoursForecast();
}
