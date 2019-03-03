using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace openWSClass
{
    public class TestClass


    {

        [DllImport(dllName: "ows.dll", CharSet = CharSet.Ansi, EntryPoint =
     "start_main_thread", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tts();




        [DllImport("Project2.dll", CharSet = CharSet.Ansi, EntryPoint =
    "testdll", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartMain();
        [DllImport("Project2.dll", CharSet = CharSet.Ansi, EntryPoint =
"testdll2", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartMain2();


        [DllImport("Project5.dll", CharSet = CharSet.Ansi, EntryPoint =
"_testdll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Test();
        [DllImport("Project5.dll", CharSet = CharSet.Ansi, EntryPoint =
"_testdll2", CallingConvention = CallingConvention.StdCall)]
        public static extern int Test2();


        public static string TestStr1()
        {

            return Environment.CurrentDirectory+Tts().ToString();
        }
    }

  
}
