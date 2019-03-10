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


             [DllImport("Testdll.dll", CharSet = CharSet.Ansi, EntryPoint =
        "text", CallingConvention = CallingConvention.Cdecl)]
             public static extern IntPtr TextOut(string str);
        //        [DllImport(dllName: "C://Users/ken13/Desktop/minpro/FFmpegTest3/WebApp/ows.dll", CharSet = CharSet.Ansi, EntryPoint =
        //        "start_main_thread", CallingConvention = CallingConvention.StdCall)]
        //        public static extern int Tts();




        [DllImport("Project2.dll", CharSet = CharSet.Ansi, EntryPoint = "testdll", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartMain();
        [DllImport("Project2.dll", CharSet = CharSet.Ansi, EntryPoint = "testdll2", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartMain2();


        [DllImport("Project5.dll", CharSet = CharSet.Ansi, EntryPoint = "_testdll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Test();
        [DllImport("Project5.dll", CharSet = CharSet.Ansi, EntryPoint = "_testdll2", CallingConvention = CallingConvention.StdCall)]
        public static extern int Test2();

        [DllImport("ows.dll", CharSet = CharSet.Ansi, EntryPoint = "start_main_thread", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartModule();

        [DllImport("Project2.dll", CharSet = CharSet.Ansi, EntryPoint = "test", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr TestInput(string str);
    }
    public class TestClass
    {
        byte[] buffer = new byte[1024];

        public TestClass()
        {
            IEnumerable<string> pathList = new List<string>() { Environment.CurrentDirectory + "/shared/window/",Environment.CurrentDirectory+"/shared/mac/"};



            AddEnvironmentPaths(pathList);
        }
        

        public string Hello()
        {



            /*+ Marshal.PtrToStringAnsi(Example.TextOut("hello")*/
            //Example.Tts();

            // Example.Test2();
            //Example.Test().ToString();
            //Example.StartModule();
            //string str = Example.Test().ToString();
            //Console.WriteLine("hello world!");
            string testString = "hello this is test!";
           
            string str = Marshal.PtrToStringAnsi(Example.TestInput(testString));

            return str;// Marshal.PtrToStringAnsi(Example.TextOut("hello"));
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
        public string GetName()
        {
            return name;
        }
        static void AddEnvironmentPaths(IEnumerable<string> paths)
        {
            var path = new[] { Environment.GetEnvironmentVariable("PATH") ?? string.Empty };    

            string newPath = string.Join(Path.PathSeparator.ToString(), path.Concat(paths));

            Environment.SetEnvironmentVariable("PATH", newPath);
            Environment.SetEnvironmentVariable("DYLD_LIBRARY_PATH", newPath);
        }
        public string Name { get; set; }
    }



}
