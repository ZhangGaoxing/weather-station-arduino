using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using SkiaSharp;
using Utility;
using Utility.Models;
using ControllerBase = Microsoft.AspNetCore.Mvc.ControllerBase;

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

        private void GenerateChart()
        {
            var hoursData = _context.Select6HourData();

            DateTimeAxis dateAxis = new DateTimeAxis
            {
                Position = AxisPosition.Bottom,
                Minimum = DateTimeAxis.ToDouble(hoursData[0].DateTime),
                Maximum = DateTimeAxis.ToDouble(hoursData[5].DateTime),
                StringFormat = "MM/dd hh:mm"
            };

            MemoryStream temp = Svg2Png(GetTempModel(dateAxis, hoursData));
        }

        private PlotModel GetTempModel(DateTimeAxis dateAxis, List<Weather> hoursData)
        {
            PlotModel tempModel = new PlotModel { Title = "环境温度" };
            tempModel.Axes.Add(
                new LinearAxis
                {
                    Title = "温度（℃）",
                    Position = AxisPosition.Left,
                    Minimum = hoursData.Min(x => x.Temperature) - 2,
                    Maximum = hoursData.Max(x => x.Temperature) + 2
                });
            tempModel.Axes.Add(dateAxis);

            var tempSeries = new LineSeries
            {
                Color = OxyColor.FromRgb(194, 53, 49),
                MarkerFill = OxyColor.FromRgb(0, 0, 0),
                MarkerType = MarkerType.Circle
            };

            foreach (var item in hoursData)
            {
                tempSeries.Points.Add(new DataPoint(DateTimeAxis.ToDouble(item.DateTime), item.Temperature));
            }

            tempModel.Series.Add(tempSeries);

            return tempModel;
        }

        private MemoryStream Svg2Png(PlotModel model)
        {
            SvgExporter exporter = new SvgExporter { Width = 600, Height = 400 };
            var svg = new SkiaSharp.Extended.Svg.SKSvg();
            var imgQuality = 80;

            MemoryStream pngStream = new MemoryStream();

            using (MemoryStream ms = new MemoryStream())
            {
                exporter.Export(model, ms);

                var pict = svg.Load(ms);

                var dimen = new SkiaSharp.SKSizeI(
                    (int)Math.Ceiling(pict.CullRect.Width),
                    (int)Math.Ceiling(pict.CullRect.Height)
                );
                var matrix = SKMatrix.MakeScale(1, 1);
                var img = SKImage.FromPicture(pict, dimen, matrix);

                // convert to PNG
                var skdata = img.Encode(SKEncodedImageFormat.Png, imgQuality);

                skdata.SaveTo(pngStream);
            }

            return pngStream;
        }
    }
}