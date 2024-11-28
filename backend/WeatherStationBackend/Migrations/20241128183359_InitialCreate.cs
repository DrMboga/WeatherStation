using Microsoft.EntityFrameworkCore.Migrations;
using Npgsql.EntityFrameworkCore.PostgreSQL.Metadata;

#nullable disable

namespace WeatherStationBackend.Migrations
{
    /// <inheritdoc />
    public partial class InitialCreate : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Sensors",
                columns: table => new
                {
                    MeasurementTimeStamp = table.Column<long>(type: "bigint", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    OutsideTemperature = table.Column<decimal>(type: "numeric", nullable: false),
                    OutsideHumidity = table.Column<decimal>(type: "numeric", nullable: false),
                    Room1Temperature = table.Column<decimal>(type: "numeric", nullable: false),
                    Room1Humidity = table.Column<decimal>(type: "numeric", nullable: false),
                    Room2Temperature = table.Column<decimal>(type: "numeric", nullable: false),
                    Room2Humidity = table.Column<decimal>(type: "numeric", nullable: false),
                    Room3Temperature = table.Column<decimal>(type: "numeric", nullable: false),
                    Room3Humidity = table.Column<decimal>(type: "numeric", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Sensors", x => x.MeasurementTimeStamp);
                });
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Sensors");
        }
    }
}
