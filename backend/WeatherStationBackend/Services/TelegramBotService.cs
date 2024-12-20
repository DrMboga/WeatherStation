using System;
using System.Text;
using System.Text.Json;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Services;

public class TelegramBotService : ITelegramBotService
{
    private readonly IDataAccess _dataAccess;
    private readonly ILogger _logger;
    private readonly IForecast _forecast;
    private readonly BackgroundWorkerSettings _settings;
    private readonly IHttpClientFactory _clientFactory;

    public TelegramBotService(
        IDataAccess dataAccess,
        ILogger<TelegramBotService> logger,
        IForecast forecast,
        BackgroundWorkerSettings settings,
        IHttpClientFactory clientFactory)
    {
        _dataAccess = dataAccess;
        _logger = logger;
        _forecast = forecast;
        _settings = settings;
        _clientFactory = clientFactory;
    }

    public async Task SendSensorsInfoAndForecast()
    {
        var latestSensorData = await _dataAccess.GetLatestData();
        var sensorsText = string.Empty;
        if(latestSensorData != null)
        {
            sensorsText = latestSensorData.ToMarkdownMessage();
            _logger.LogInformation(sensorsText);
        }
        var forecast = await _forecast.GetNextTwelveHoursForecast();
        var forecastText = forecast.ToMarkdownMessage();

        var text = string.Join(Environment.NewLine, sensorsText, forecastText);

        await SendToTelegram(text);
    }

    private async Task SendToTelegram(string text)
    {
        var client = _clientFactory.CreateClient("TelegramBotApi");
        client.Timeout = TimeSpan.FromMinutes(10);

        _logger.LogInformation(text);
        _logger.LogInformation($"{client.BaseAddress}bot{_settings.TelegramBotToken}/sendMessage");

        var body = new {
            parse_mode = "HTML",
            text,
            chat_id = _settings.TelegramChatId
        };
        var content = new StringContent(JsonSerializer.Serialize(body), Encoding.UTF8, "application/json");
        var response = await client.PostAsync($"{client.BaseAddress}bot{_settings.TelegramBotToken}/sendMessage", content);
        response.EnsureSuccessStatusCode();
    }
}