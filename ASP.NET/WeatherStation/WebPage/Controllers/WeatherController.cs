using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
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

        // GET: /<controller>/
        public IActionResult Index()
        {
            var data = _context.Weathers.OrderByDescending(x => x.DateTime)
                .Take(6)
                .OrderBy(x => x.DateTime)
                .ToList();

            return View(data);
        }
    }
}
