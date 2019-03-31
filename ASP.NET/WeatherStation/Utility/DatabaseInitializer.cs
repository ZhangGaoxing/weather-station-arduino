using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Utility.Models;

namespace Utility
{
    public class DatabaseInitializer
    {
        public static Weather[] Seed()
        {
            return new Weather[]
                {
                    new Weather
                    {
                        ID=1,
                        DateTime=new DateTime(2019,1,1,21,0,0),
                        Temperature=-0.9,
                        Humidity=25.8,
                        Pressure=103662.72,
                        UV=0.01,
                        Dust=0.07
                    },
                    new Weather
                    {
                        ID=2,
                        DateTime=new DateTime(2019,1,1,22,0,0),
                        Temperature=-1.5,
                        Humidity=28.8,
                        Pressure=103729.69,
                        UV=0.01,
                        Dust=0.06
                    },
                    new Weather
                    {
                        ID=3,
                        DateTime=new DateTime(2019,1,1,23,0,0),
                        Temperature=-2.2,
                        Humidity=29.6,
                        Pressure=103632.05,
                        UV=0.01,
                        Dust=0.06
                    },
                    new Weather
                    {
                        ID=4,
                        DateTime=new DateTime(2019,1,2,0,0,0),
                        Temperature=-2.9,
                        Humidity=31.8,
                        Pressure=103884.52,
                        UV=0.01,
                        Dust=0.09
                    },
                    new Weather
                    {
                        ID=5,
                        DateTime=new DateTime(2019,1,2,1,0,0),
                        Temperature=-3.6,
                        Humidity=33.8,
                        Pressure=103603.38,
                        UV=0.01,
                        Dust=0.05
                    },
                    new Weather
                    {
                        ID=6,
                        DateTime=new DateTime(2019,1,2,2,0,0),
                        Temperature=-3.5,
                        Humidity=32.8,
                        Pressure=103704.99,
                        UV=0.01,
                        Dust=0.06
                    },
                    new Weather
                    {
                        ID=7,
                        DateTime=new DateTime(2019,1,2,3,0,0),
                        Temperature=-3.1,
                        Humidity=31.2,
                        Pressure=103872.41,
                        UV=0.01,
                        Dust=0.06
                    },
                    new Weather
                    {
                        ID=8,
                        DateTime=new DateTime(2019,1,2,4,0,0),
                        Temperature=-2.7,
                        Humidity=29.2,
                        Pressure=103882.69,
                        UV=0.01,
                        Dust=0.06
                    },
                };
        }
    }
}
