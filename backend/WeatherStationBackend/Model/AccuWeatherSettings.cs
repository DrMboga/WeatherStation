using System;

namespace WeatherStationBackend.Model;

public class AccuWeatherSettings
{
    public string ApiKey { get; set; } = string.Empty;
    public string LocationKey { get; set; } = string.Empty;
}
