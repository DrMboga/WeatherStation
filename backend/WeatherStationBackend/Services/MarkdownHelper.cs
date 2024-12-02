using System;
using System.Text;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Services;

public static class MarkdownHelper
{

    public static DateTime ToDateTime(this long unixTimeStamp)
    {
        // Unix timestamp is seconds past epoch
        var dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, DateTimeKind.Utc);
        dtDateTime = dtDateTime.AddSeconds(unixTimeStamp).ToLocalTime();
        return dtDateTime;
    }

    public static string ToMarkdownMessage(this SensorData sensorData)
    {
        var sensorsDate = sensorData.MeasurementTimeStamp.ToDateTime().ToString("dd.MM.yyyy HH:mm");
        var stringBuilder = new StringBuilder();
        stringBuilder.AppendLine($"## The home weather station wishes you a good {TimeOfTheDay()}");
        stringBuilder.AppendLine($"### On {sensorsDate}:");
        stringBuilder.AppendLine($" - It's: {Convert.ToInt32(sensorData.Outside?.Temperature ?? 0)}℃ outside, humidity {Convert.ToInt32(sensorData.Outside?.Humidity ?? 0)}%");
        stringBuilder.AppendLine($" - In the first room {Convert.ToInt32(sensorData.Room1?.Temperature ?? 0)}℃, humidity {Convert.ToInt32(sensorData.Room1?.Humidity ?? 0)}%");
        stringBuilder.AppendLine($" - In the second room {Convert.ToInt32(sensorData.Room2?.Temperature ?? 0)}℃, humidity {Convert.ToInt32(sensorData.Room2?.Humidity ?? 0)}%");
        stringBuilder.AppendLine($" - In the third room: {Convert.ToInt32(sensorData.Room3?.Temperature ?? 0)}℃, humidity {Convert.ToInt32(sensorData.Room3?.Humidity ?? 0)}%");
        stringBuilder.AppendLine("---");
        stringBuilder.AppendLine();
        return stringBuilder.ToString();
    }

    public static string ToMarkdownMessage(this AccuWeatherHourlyForecastResponse[] forecast)
    {
        var stringBuilder = new StringBuilder();
        stringBuilder.AppendLine("### Weather forecast for the next 12 hours:");

        var minTemp = Convert.ToInt32(forecast.Min(f => f.Temperature?.Value ?? 0));
        var maxTemp = Convert.ToInt32(forecast.Max(f => f.Temperature?.Value ?? 0));
        stringBuilder.AppendLine($" - Temperature from {minTemp}℃ to {maxTemp}℃");

        var precipitationHourly = forecast
            .Where(f => f.HasPrecipitation)
            .Select(f => new { Time = f.EpochDateTime.ToDateTime().ToShortTimeString(), Probability = f.PrecipitationProbability })
            .ToArray();
        if (precipitationHourly == null || precipitationHourly.Length == 0)
        {
            stringBuilder.AppendLine(" - No precipitation is expected");
        }
        else
        {
            foreach (var precipitationInfo in precipitationHourly)
            {
                stringBuilder.AppendLine($" - Precipitation is expected at {precipitationInfo.Time} with a probability of {precipitationInfo.Probability}%.");
            }
        }

    stringBuilder.AppendLine();

        return stringBuilder.ToString();
    }

    private static string TimeOfTheDay()
    {
        var now = DateTime.Now;
        return now.Hour switch
        {
            int d when (d >= 6 && d <= 11) => "morning",
            int d when (d > 11 && d <= 17) => "day",
            int d when (d > 17 && d <= 23) => "evening",
            _ => "night",
        };
    }
}