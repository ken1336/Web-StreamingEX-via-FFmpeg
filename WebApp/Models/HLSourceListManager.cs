using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WebApp.Models
{
    public class HLSourceListManager
    {
        private static string PathURL = Environment.CurrentDirectory;


        public HLSourceListManager(string DirectoryID)
        {
            PathURL += "/" + DirectoryID; 
        }
    }
}
