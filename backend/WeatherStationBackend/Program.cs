using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using WeatherStationBackend.Data;
using WeatherStationBackend.Model;
using WeatherStationBackend.Services;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddDbContextFactory<WeatherStationContext>(
    options =>
        options.UseNpgsql(builder.Configuration.GetConnectionString("weather")));

builder.Services.AddTransient<IDataAccess, DataAccess>();

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


app.Run();
