using System;

namespace WeatherStationBackend.Model;

public class AccuWeatherHourlyForecastResponse
{
    public string DateTime { get; set; } = new DateTime().ToString();
    public long EpochDateTime { get; set; }
    public int WeatherIcon { get; set; }
    public string IconPhrase { get; set; } = string.Empty;
    public bool HasPrecipitation { get; set; }
    public bool IsDaylight { get; set; }
    public AccuWeatherTemperature? Temperature { get; set; }
    public decimal PrecipitationProbability { get; set; }
    public string MobileLink { get; set; }= string.Empty;
    public string Link { get; set; } = string.Empty;

}
