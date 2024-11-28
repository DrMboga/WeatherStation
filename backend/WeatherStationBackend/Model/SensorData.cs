using System;

namespace WeatherStationBackend.Model;

/// <summary>
/// Represents the sensors measurement data
/// </summary>
public class SensorData
{
    /// <summary>
    /// Measurement date as Unix timestamp
    /// </summary>
    public long MeasurementTimeStamp { get; set; }
    /// <summary>
    /// Outside sensor data
    /// </summary>
    public SensorInfo? Outside { get; set; }
    /// <summary>
    /// Sensor data from room one
    /// </summary>
    public SensorInfo? Room1 { get; set; }
    /// <summary>
    /// Sensor data from room two
    /// </summary>
    public SensorInfo? Room2 { get; set; }
    /// <summary>
    /// Sensor data from room three
    /// </summary>
    public SensorInfo? Room3 { get; set; }
}
