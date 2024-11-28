using System.Text.Json;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using WeatherStationBackend.Data;
using WeatherStationBackend.Model;
using WeatherStationBackend.Services;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContextFactory<WeatherStationContext>(
    options =>
        options.UseNpgsql(builder.Configuration.GetConnectionString("weather")));

builder.Services.AddTransient<DataAccess>();

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

app.MapPost("/savesensorreadings", ([FromBody]SensorData sensorData, DataAccess dataAccess) => {
    app.Logger.LogInformation($"Saved new sensor readings: '{JsonSerializer.Serialize(sensorData)}'");
    return Results.Ok();
})
.WithName("SaveSensorReadings")
.WithOpenApi();

// http://localhost:5119/getsensordata?dateFrom=56
app.MapGet("/getsensordata", (long dateFrom, DataAccess dataAccess) => {
    app.Logger.LogInformation($"Trying to get sensor data from: {dateFrom}");
    var stubData = @"
    [
    {""MeasurementTimeStamp"":1732812807,""Outside"":{""Temperature"":8,""Humidity"":45},""Room1"":{""Temperature"":18,""Humidity"":55},""Room2"":{""Temperature"":19,""Humidity"":54},""Room3"":{""Temperature"":20,""Humidity"":50.1}},
    {""MeasurementTimeStamp"":1732815052,""Outside"":{""Temperature"":8.45,""Humidity"":45.33},""Room1"":{""Temperature"":18.50,""Humidity"":55.1},""Room2"":{""Temperature"":19,""Humidity"":54},""Room3"":{""Temperature"":20,""Humidity"":50.1}}
    ]
    ";
    var sensorData = JsonSerializer.Deserialize<SensorData[]>(stubData);
    return Results.Ok<SensorData[]>(sensorData);
})
.WithName("GetSensorData")
.WithOpenApi();


app.Run();
