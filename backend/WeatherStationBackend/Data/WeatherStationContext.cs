using System;
using Microsoft.EntityFrameworkCore;

namespace WeatherStationBackend.Data;

public class WeatherStationContext: DbContext
{
    // TODO: For migrations uncomment this constructor and comment out another one. Then run:
    // dotnet ef migrations add InitialCreate
    // dotnet ef database update
    // protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    // {
    //     optionsBuilder.UseNpgsql("Host=localhost;Database=weather;Username=postgres;Password=PuyLjs9jeChfJRF");
    // }

    public WeatherStationContext(DbContextOptions<WeatherStationContext> options): base(options)
    {
    }

    public virtual DbSet<SensorDataModel> Sensors { get; set; } = null;

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<SensorDataModel>(entity => {
            entity.HasKey(e => e.MeasurementTimeStamp);
        });
    }
}
