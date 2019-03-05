using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace WebApp.Models.Contexts
{
    public class StreaminContext
    {

        [DllImport("ows.dll", CharSet = CharSet.Ansi, EntryPoint = "start_main_thread", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartModule();


        private static string sourceDirectory = Environment.CurrentDirectory + "/media/";
        private string ServerURL { get; set; }
        private uint port { get; set; }
        private string contextID { get; set; }
        private string sourceURL { get; set; }

    }
}
