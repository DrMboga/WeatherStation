using System;

namespace WeatherStationBackend.Model;

public class AccuWeatherLocationResponse
{
    public string Key { get; set; } = string.Empty;
    public int Rank { get; set; } = 0;
    public string LocalizedName { get; set; } = string.Empty;
    public string EnglishName { get; set; } = String.Empty;
    public AccuWeatherCountry? Country { get; set; }
}
