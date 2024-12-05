using System.Net;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Quartz;
using WeatherStationBackend.Data;
using WeatherStationBackend.Jobs;
using WeatherStationBackend.Model;
using WeatherStationBackend.Services;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContextFactory<WeatherStationContext>(
    options =>
        options.UseNpgsql(builder.Configuration.GetConnectionString("weather")));

builder.Services.AddTransient<IDataAccess, DataAccess>();

// AccuWeather Api service
builder.Services.AddHttpClient("AccuWeatherApi", c =>
{
    c.BaseAddress = new Uri("http://dataservice.accuweather.com/");
    c.DefaultRequestHeaders.Add("Accept", "*/*");
    c.DefaultRequestHeaders.Add("User-Agent", "Mike's home Weather Station");
});

// Telegram bot api
builder.Services.AddHttpClient("TelegramBotApi", c =>
{
    c.BaseAddress = new Uri("https://api.telegram.org/");
    c.DefaultRequestHeaders.Add("Accept", "application/json");
    c.DefaultRequestHeaders.Add("User-Agent", "Mike's home Weather Station");
});

// App settings
var accuWeatherSettings = builder.Configuration
                .GetSection("AccuWeather")
                .Get<AccuWeatherSettings>();
builder.Services.AddSingleton<AccuWeatherSettings>(accuWeatherSettings!);
var backgroundWorkerSettings = builder.Configuration
                .GetSection("BackgroundWorker")
                .Get<BackgroundWorkerSettings>();
builder.Services.AddSingleton<BackgroundWorkerSettings>(backgroundWorkerSettings!);       

builder.Services.AddTransient<IForecast, Forecast>();
builder.Services.AddTransient<ITelegramBotService, TelegramBotService>();

// Quartz scheduler
builder.Services.AddQuartz(q =>
{
    var jobKey = new JobKey("SendWeatherInfoToTelegram");
    q.AddJob<TelegramBotJob>(opts => opts.WithIdentity(jobKey));
    q.AddTrigger(opts => opts
        .ForJob(jobKey)
        .WithIdentity("weather-info-working-day-trigger")
         //This Cron interval can be described as "Run every working day at 6:30"
        .WithCronSchedule("0 30 6 ? * MON,TUE,WED,THU,FRI *")
    );
    q.AddTrigger(opts => opts
        .ForJob(jobKey)
        .WithIdentity("weather-info-weekend-trigger")
         //This Cron interval can be described as "Run every weekend day at 8:00"
        .WithCronSchedule("0 0 8 ? * SUN,SAT *")
    );
});
builder.Services.AddQuartzHostedService(q => q.WaitForJobsToComplete = true);

// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
// http://localhost:5119/swagger/index.html
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.MapPost("/savesensorreadings", async ([FromBody]SensorData sensorData, IDataAccess dataAccess) => {
    await dataAccess.SaveSensorReadings(sensorData);
    return Results.Ok();
})
.WithName("SaveSensorReadings")
.WithOpenApi();

// http://localhost:5119/getsensordata?dateFrom=56
app.MapGet("/getsensordata", async (long dateFrom, IDataAccess dataAccess) => {
    var sensorData = await dataAccess.GetSensorData(dateFrom);
    return Results.Ok<SensorData[]>(sensorData);
})
.WithName("GetSensorData")
.WithOpenApi();

// /getweatherforecastlocations?postalCode=66111
app.MapGet("/getweatherforecastlocations", async(string postalCode, IForecast forecast) => {
    var locations = await forecast.GetLocationByPostalCode(postalCode);
    return Results.Ok(locations);
})
.WithName("GetWeatherForecastLocations")
.WithOpenApi();

app.MapGet("/getweatherforecastnexthour", async(IForecast forecast) => {
    try
    {
        var hourly = await forecast.GetNextHourForecast();
        return Results.Ok(hourly);
    } catch(HttpRequestException e){
        // Error 503 from AccuWeather means that we exceeded daily requests limit for free tier
        if (e.StatusCode == HttpStatusCode.ServiceUnavailable) {
            return Results.StatusCode(503);
        }
        return Results.BadRequest(e.Message);
    }
})
.WithName("GetWeatherForecastNextHour")
.WithOpenApi();

app.MapGet("/getweatherforecastnexttwelvehours", async(IForecast forecast) => {
    var hourly = await forecast.GetNextTwelveHoursForecast();
    return Results.Ok(hourly);
})
.WithName("GetWeatherForecastNextTwelveHours")
.WithOpenApi();

app.MapPost("/sendinfototelegram", async(ITelegramBotService telegram) => {
    await telegram.SendSensorsInfoAndForecast();
    return Results.Ok();
})
.WithName("SendInfoToTelegram")
.WithOpenApi();


app.Run();
