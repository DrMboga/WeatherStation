using System;

namespace WeatherStationBackend.Model;

public class BackgroundWorkerSettings
{
    public string TelegramBotToken { get; set; } = string.Empty;
    public string TelegramChatId { get; set; } = string.Empty;
}
