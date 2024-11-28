using System;
using Microsoft.EntityFrameworkCore;
using WeatherStationBackend.Data;

namespace WeatherStationBackend.Services;

public class DataAccess
{
    private readonly IDbContextFactory<WeatherStationContext> _dbContextFactory;

    public DataAccess(IDbContextFactory<WeatherStationContext> dbContextFactory)
    {
        _dbContextFactory = dbContextFactory;
    }

}
