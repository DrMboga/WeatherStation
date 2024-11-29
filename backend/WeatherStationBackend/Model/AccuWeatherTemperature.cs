using System;

namespace WeatherStationBackend.Model;

public class AccuWeatherTemperature
{
    public decimal Value { get; set; }
    public string Unit { get; set; } = string.Empty;
}
