using System;

namespace WeatherStationBackend.Services;

public interface ITelegramBotService
{
    /// <summary>
    /// Gets latest sensors data from DB, combines it with 12-hours forecast from AccuWeather and sends the info to Telegram chat
    /// </summary>
    Task SendSensorsInfoAndForecast();
}
