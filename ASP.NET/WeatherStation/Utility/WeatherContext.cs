using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Utility.Models;

namespace Utility
{
    public class WeatherContext : DbContext
    {
        public DbSet<Weather> Weathers { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            // Connection String
            optionsBuilder.UseNpgsql("Server=localhost;Port=5432;Database=WeatherStation;User Id=postgres;Password=YOUR PASSWORD;");
                //.UseSqlServer(@"Server=(localdb)\MSSQLLocalDB;Initial Catalog=WeatherStation;User ID=YOUR USER ID;Password=YOUR PASSWORD"); 
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Weather>().ToTable("weather");

            modelBuilder.Entity<Weather>().HasData(DatabaseInitializer.Seed());
        }

        public List<Weather> Select6HourData()
        {
            List<Weather> hoursData = new List<Weather>();

            var groups = Weathers.GroupBy(x => x.DateTime.Date)
                .OrderByDescending(x => x.Key)
                .Take(2)
                .Select(g => g.Select(x => x).ToList()).ToList();

            var today = groups[0].GroupBy(x => x.DateTime.Hour)
                .OrderByDescending(x => x.Key)
                .Select(g => g.Select(x => x).ToList()).ToList();

            int count = today.Count;
            if (count >= 6)
            {
                var temp = today.Take(6);

                foreach (var item in temp)
                {
                    hoursData.Add(item[0]);
                }
            }
            else
            {
                var yesterday = groups[1].GroupBy(x => x.DateTime.Hour)
                    .OrderByDescending(x => x.Key)
                    .Select(g => g.Select(x => x).ToList()).ToList();

                var temp = yesterday.Take(6 - count);

                foreach (var item in today)
                {
                    hoursData.Add(item[0]);
                }

                foreach (var item in temp)
                {
                    hoursData.Add(item[0]);
                }
            }

            hoursData.Reverse();

            return hoursData;
        }
    }
}
