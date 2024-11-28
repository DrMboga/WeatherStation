using System;

namespace WeatherStationBackend.Data;

public class SensorDataModel
{
    public long MeasurementTimeStamp { get; set; }
    public decimal OutsideTemperature { get; set; }
    public decimal OutsideHumidity { get; set; }
    public decimal Room1Temperature { get; set; }
    public decimal Room1Humidity { get; set; }
    public decimal Room2Temperature { get; set; }
    public decimal Room2Humidity { get; set; }
    public decimal Room3Temperature { get; set; }
    public decimal Room3Humidity { get; set; }

}
