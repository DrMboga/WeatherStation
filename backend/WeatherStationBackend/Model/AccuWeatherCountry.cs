using System;

namespace WeatherStationBackend.Model;

public class AccuWeatherCountry
{
    public string ID { get; set; } = string.Empty;
    public string LocalizedName { get; set;} = string.Empty;
    public string EnglishName { get; set;} = string.Empty;
}
