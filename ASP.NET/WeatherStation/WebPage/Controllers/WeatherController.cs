using System.Linq;
using Microsoft.AspNetCore.Mvc;
using Utility;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace WebPage.Controllers
{
    public class WeatherController : Controller
    {
        private readonly WeatherContext _context;

        public WeatherController(WeatherContext context)
        {
            _context = context;
        }

        [HttpGet]
        public IActionResult Index()
        {
            // Select Latest Data
            ViewData["LatestData"] = _context.Weathers.OrderByDescending(x => x.DateTime)
                .First();

            // Select 6 Hour Data
            ViewData["HoursData"] = _context.Select6HourData();

            return View();
        }
    }
}
