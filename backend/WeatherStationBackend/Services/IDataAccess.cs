using System;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Services;

public interface IDataAccess
{
    Task SaveSensorReadings(SensorData sensorData);

    Task<SensorData[]> GetSensorData(long dateFrom);

    Task<SensorData?> GetLatestData();
}
