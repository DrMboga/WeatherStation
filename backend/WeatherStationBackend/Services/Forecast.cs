using System;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Services;

public class Forecast : IForecast
{
    private readonly AccuWeatherSettings _settings;
    private readonly IHttpClientFactory _accuWeatherClientFactory;

    public Forecast(AccuWeatherSettings settings, IHttpClientFactory accuWeatherClientFactory)
    {
        _settings = settings;
        _accuWeatherClientFactory = accuWeatherClientFactory;
    }

    public async Task<AccuWeatherLocationResponse[]> GetLocationByPostalCode(string postalCode)
    {
        var url = $"locations/v1/postalcodes/search?apikey={_settings.ApiKey}&q={postalCode}";
        var data = await GetFromAccuWeather<AccuWeatherLocationResponse[]>(url);
        return data ?? [];
    }

    public async Task<AccuWeatherHourlyForecastResponse> GetNextHourForecast()
    {
        var url = $"forecasts/v1/hourly/1hour/{_settings.LocationKey}?apikey={_settings.ApiKey}&metric=true";
        var data = await GetFromAccuWeather<AccuWeatherHourlyForecastResponse[]>(url);
        if (data == null || data.Length == 0)
        {
            return new AccuWeatherHourlyForecastResponse();
        }
        return data[0];
    }

    public async Task<AccuWeatherHourlyForecastResponse[]> GetNextTwelveHoursForecast()
    {
        var url = $"forecasts/v1/hourly/12hour/{_settings.LocationKey}?apikey={_settings.ApiKey}&metric=true";
        var data = await GetFromAccuWeather<AccuWeatherHourlyForecastResponse[]>(url);
        return data ?? [];
    }

    private async Task<T?> GetFromAccuWeather<T>(string url)
    {
        var client = _accuWeatherClientFactory.CreateClient("AccuWeatherApi");

        var response = await client.GetAsync(url);
        response.EnsureSuccessStatusCode();

        var data = await response.Content.ReadFromJsonAsync<T>();

        return data;
    }
}
