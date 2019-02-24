using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.IO;
using System.Windows;
namespace WebApp.Models
{
    public class TestClass
    {
        byte[] buffer = new byte[1024];

        public int Add(int a, int b)
        {

            return a + b;
        }

        public string Hello()
        {
            return "hello world!";
        }
        public string foo()
        {
            const string url = "C://Users/ken13/Desktop/media/frame1.jpg";



            Task<int> tk;


            using (var readFile = new FileStream(url, FileMode.Open))
            {
                using (var outfile = new FileStream(Environment.CurrentDirectory + "/wwwroot/test2.jpg", FileMode.Create))
                {

                    while ((tk = readFile.ReadAsync(buffer, 0, 1024)).Result > 0)
                    {
                        outfile.Write(buffer, 0, tk.Result);
                        outfile.Flush();
                    }
                }
            }
            
            
            
            return "파일쓰기 성공";
        }
        private string name;
        public string getName()
        {
            return name;
        }

        public string Name { get; set; }
    }



}
