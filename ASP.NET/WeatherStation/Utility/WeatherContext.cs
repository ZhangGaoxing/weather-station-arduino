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
            optionsBuilder.UseSqlServer(@"Server=(localdb)\MSSQLLocalDB;Initial Catalog=WeatherStation;User ID=YOUR USER ID;Password=YOUR PASSWORD");
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Weather>().ToTable("Weathers");

            modelBuilder.Entity<Weather>().HasData(DatabaseInitializer.Seed());
        }

    }
}
