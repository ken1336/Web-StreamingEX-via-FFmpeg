using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using WebApp.Models;

namespace WebApp.Controllers
{
    public class HomeController : Controller
    {
        //[HttpGet]
        //public FileResult DownloadSample()
        //{
        //    // 웹서버 상의 파일 절대 경로 구함
           
        //    string filePath ="~/media/ttest.m3u8";

        //    // 파일 읽기
        //    byte[] fileBytes = System.IO.File.ReadAllBytes(filePath);

        //    // FileResult 리턴
        //  //  return File(fileBytes, System.Net.Mime.MediaTypeNames.Application.Octet, Path.GetFileName(filePath));
        //}
        public IActionResult Main()
        {
            TestClass min = new TestClass();
            Console.Write(min.Hello());
            ViewData["text"] = min.Hello();  
            return View();
        }
        public IActionResult Index()
        {
            return View();
        }

        public IActionResult About()
        {
            ViewData["Message"] = "Your application description page.";

            return View();
        }

        public IActionResult Contact()
        {
            ViewData["Message"] = "Your contact page.";

            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
