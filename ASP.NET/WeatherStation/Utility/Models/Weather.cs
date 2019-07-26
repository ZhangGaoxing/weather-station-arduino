using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Threading.Tasks;

namespace Utility.Models
{
    public class Weather
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        [Column("weather_id")]
        public long WeatherID { get; set; }

        [Column("date_time")]
        public DateTime DateTime { get; set; }

        [Column("temperature")]
        public double Temperature { get; set; }

        [Column("humidity")]
        public double Humidity { get; set; }

        [Column("pressure")]
        public double Pressure { get; set; }

        [Column("dust")]
        public double Dust { get; set; }

        [Column("uv")]
        public double UV { get; set; }
    }
}
