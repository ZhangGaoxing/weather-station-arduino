using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Utility;
using Utility.Models;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class WeatherController : ControllerBase
    {
        private readonly WeatherContext _context;

        public WeatherController(WeatherContext context)
        {
            _context = context;
        }

        [HttpGet]
        public Weather Get(int id = 1)
        {
            try
            {
                return _context.Weathers.Where(x => x.ID == id).First();
            }
            catch (Exception)
            {
                return null;
            }
        }

        [HttpPost]
        public async Task<string> Post(string token, Weather weather)
        {
            if (token != "xxxxx")
            {
                return "Token error.";
            }

            try
            {
                _context.Weathers.Add(weather);
                await _context.SaveChangesAsync();

                return "True";
            }
            catch (Exception)
            {
                return "False";
            }
        }
    }
}