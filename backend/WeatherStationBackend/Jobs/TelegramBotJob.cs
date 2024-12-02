using System;
using Quartz;
using WeatherStationBackend.Services;

namespace WeatherStationBackend.Jobs;

public class TelegramBotJob : IJob
{
    private readonly ITelegramBotService _telegramBotService;

    public TelegramBotJob(ITelegramBotService telegramBotService)
    {
        _telegramBotService = telegramBotService;
    }

    public async Task Execute(IJobExecutionContext context)
    {
        await _telegramBotService.SendSensorsInfoAndForecast();
    }
}
