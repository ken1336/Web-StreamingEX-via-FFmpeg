

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.IO;
using System.Windows;

using System;
using System.Runtime.InteropServices;
namespace WebApp.Models
{
    class Example
    {
    
        [DllImport("C://Users/ken13/Desktop/minpro/FFmpegTest3/WebApp/TestDll.dll", CharSet = CharSet.Unicode,EntryPoint =
       "math_add1", CallingConvention = CallingConvention.StdCall)]
        public static extern int Add(int a, int b);

        [DllImport("C://Users/ken13/Desktop/minpro/FFmpegTest3/WebApp/TestDll.dll", CharSet = CharSet.Unicode, EntryPoint =
      "text", CallingConvention = CallingConvention.Cdecl)]
        public static extern string TextOut(ref char str);




    }
    public class TestClass
    {
        byte[] buffer = new byte[1024];

       

        public string Hello()
        {

            Char text = "Hello\0".ToArray()[0];


            return Example.Add(3,1).ToString()+ Example.TextOut(ref text);
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
