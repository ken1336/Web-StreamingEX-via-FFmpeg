using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace WebApp.Models
{
    public class LogClass
    {
        static string LogPath = Environment.CurrentDirectory + "/log.txt";
        
        public static void LogWrite(string str)
        {
     


            using (var OutFile = new StreamWriter(LogPath,true))
            {
                
                
                OutFile.WriteLine(str);
                OutFile.Flush();
                 
                
            }
        }
        public static void LogWrite(byte[] str)
        {



            using (var OutFile = new FileStream(LogPath,FileMode.Append))
            {


                OutFile.WriteAsync(str);
                OutFile.Flush();


            }
        }
    }
}
