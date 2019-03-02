using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
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
        public async Task<string> Post([FromQuery]string token, [FromQuery]Weather weather)
        {
            if (token != "xxxxx")
            {
                return "Token error.";
            }

            try
            {
                _context.Weathers.Add(weather);
                await _context.SaveChangesAsync();

                if (weather.DateTime.Minute == 0 || weather.DateTime.Minute == 30)
                {
                    using(HttpClient client=new HttpClient())
                    {
                        using(HttpContent content=new StringContent(""))
                        {
                            content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("application/x-www-form-urlencoded");

                            HttpResponseMessage response = await client.PostAsync($"https://api.weibo.com/2/statuses/share.json?access_token=2.005MlWeH0GByfS2b6ddd6bb9jLrEeC&status=" +
                                $"时间：{weather.DateTime.ToString("yyyy/MM/dd HH:mm")}%0a" +
                                $"温度：{Math.Round(weather.Temperature, 1)} ℃%0a" +
                                $"相对湿度：{Math.Round(weather.Humidity)} %25%0a" +
                                $"气压：{Math.Round(weather.Pressure)} Pa%0a" +
                                $"可吸入颗粒物：{weather.Dust} mg%2fm3%0a" +
                                $"http://maestrobot.cn", content);
                        }
                    }
                }

                return "True";
            }
            catch (Exception)
            {
                return "False";
            }
        }
    }
}