using System;
using Microsoft.EntityFrameworkCore;
using WeatherStationBackend.Data;
using WeatherStationBackend.Mappers;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Services;

public class DataAccess: IDataAccess
{
    private readonly IDbContextFactory<WeatherStationContext> _dbContextFactory;

    public DataAccess(IDbContextFactory<WeatherStationContext> dbContextFactory)
    {
        _dbContextFactory = dbContextFactory;
    }

    public async Task<SensorData[]> GetSensorData(long dateFrom)
    {
        using(var context = _dbContextFactory.CreateDbContext())
        {
            // seems that this is a first db query, so it should run migrations
            await context.Database.MigrateAsync();
            var sensorDataModel = await context.Sensors
                .AsNoTracking()
                .Where(s => s.MeasurementTimeStamp >= dateFrom)
                .ToArrayAsync();
            if (sensorDataModel == null)
            {
                return [];
            }
            return sensorDataModel.ToSensorDataArray();
        }
    }

    public async Task SaveSensorReadings(SensorData sensorData)
    {
        var sensorDataModel = sensorData.ToSensorDataModel();
        using (var context = _dbContextFactory.CreateDbContext())
        {
            await context.Sensors.AddAsync(sensorDataModel);
            await context.SaveChangesAsync();
        }
    }

    public async Task<SensorData?> GetLatestData()
    {
        using(var context = _dbContextFactory.CreateDbContext())
        {
            var latestData = await context.Sensors
                .AsNoTracking()
                .OrderByDescending(s => s.MeasurementTimeStamp)
                .Take(1)
                .FirstOrDefaultAsync();
            if (latestData == null)
            {
                return null;
            }

            return latestData.ToSensorData();
        }
    }
}
