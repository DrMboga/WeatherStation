using System;
using WeatherStationBackend.Data;
using WeatherStationBackend.Model;

namespace WeatherStationBackend.Mappers;

public static class SensorDataMapper
{
    public static SensorDataModel ToSensorDataModel(this SensorData sensorData)
    {
        return new SensorDataModel 
        {
            MeasurementTimeStamp = sensorData.MeasurementTimeStamp,
            OutsideTemperature= sensorData.Outside?.Temperature ?? 0.0m,
            OutsideHumidity= sensorData.Outside?.Humidity ?? 0.0m,
            Room1Temperature= sensorData.Room1?.Temperature ?? 0.0m,
            Room1Humidity= sensorData.Room1?.Humidity ?? 0.0m,
            Room2Temperature= sensorData.Room2?.Temperature ?? 0.0m,
            Room2Humidity= sensorData.Room2?.Humidity ?? 0.0m,
            Room3Temperature= sensorData.Room3?.Temperature ?? 0.0m,
            Room3Humidity= sensorData.Room3?.Humidity ?? 0.0m,
        };
    }

    public static SensorData[] ToSensorDataArray(this SensorDataModel[] sensorDataModel)
    {
        var output = new List<SensorData>();
        foreach (var item in sensorDataModel)
        {
            output.Add(ToSensorData(item));
        }

        return [..output];
    }
    public static SensorData ToSensorData(this SensorDataModel sensorDataModel)
    {
        return new SensorData{
            MeasurementTimeStamp = sensorDataModel.MeasurementTimeStamp,
            Outside = new SensorInfo {
                Temperature = sensorDataModel.OutsideTemperature,
                Humidity = sensorDataModel.OutsideHumidity
            },
            Room1 = new SensorInfo {
                Temperature = sensorDataModel.Room1Temperature,
                Humidity = sensorDataModel.Room1Humidity
            },
            Room2 = new SensorInfo {
                Temperature = sensorDataModel.Room2Temperature,
                Humidity = sensorDataModel.Room2Humidity
            },
            Room3 = new SensorInfo {
                Temperature = sensorDataModel.Room3Temperature,
                Humidity = sensorDataModel.Room3Humidity
            },
        };
    }
}
