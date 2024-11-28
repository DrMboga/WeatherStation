﻿// <auto-generated />
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;
using Npgsql.EntityFrameworkCore.PostgreSQL.Metadata;
using WeatherStationBackend.Data;

#nullable disable

namespace WeatherStationBackend.Migrations
{
    [DbContext(typeof(WeatherStationContext))]
    partial class WeatherStationContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("ProductVersion", "9.0.0")
                .HasAnnotation("Relational:MaxIdentifierLength", 63);

            NpgsqlModelBuilderExtensions.UseIdentityByDefaultColumns(modelBuilder);

            modelBuilder.Entity("WeatherStationBackend.Data.SensorDataModel", b =>
                {
                    b.Property<long>("MeasurementTimeStamp")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("bigint");

                    NpgsqlPropertyBuilderExtensions.UseIdentityByDefaultColumn(b.Property<long>("MeasurementTimeStamp"));

                    b.Property<decimal>("OutsideHumidity")
                        .HasColumnType("numeric");

                    b.Property<decimal>("OutsideTemperature")
                        .HasColumnType("numeric");

                    b.Property<decimal>("Room1Humidity")
                        .HasColumnType("numeric");

                    b.Property<decimal>("Room1Temperature")
                        .HasColumnType("numeric");

                    b.Property<decimal>("Room2Humidity")
                        .HasColumnType("numeric");

                    b.Property<decimal>("Room2Temperature")
                        .HasColumnType("numeric");

                    b.Property<decimal>("Room3Humidity")
                        .HasColumnType("numeric");

                    b.Property<decimal>("Room3Temperature")
                        .HasColumnType("numeric");

                    b.HasKey("MeasurementTimeStamp");

                    b.ToTable("Sensors");
                });
#pragma warning restore 612, 618
        }
    }
}
