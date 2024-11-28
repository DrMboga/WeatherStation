using System;

namespace WeatherStationBackend.Model;

/// <summary>
/// Represents the unit of measurement
/// </summary>
public class SensorInfo
{
    /// <summary>
    /// Temperature in celsius
    /// </summary>
    public decimal Temperature { get; set; }
    /// <summary>
    /// Air humidity
    /// </summary>
    public decimal Humidity { get; set;}
}
