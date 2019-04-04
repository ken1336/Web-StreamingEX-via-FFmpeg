using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace WebApp.Models.Native
{
   
    public class RawDataReceiveClass : HLSourceListManager
    {
        Socket Sock;
        Thread StreamIOThread;
        byte[] buffer;
        IntPtr ptr;
        public RawDataReceiveClass(Socket sock) : base("TEST")
        {
            
            this.Sock = sock;
            buffer = new byte[32 * 1024 * 1024];
            Thread StreamIOThread = new Thread(new ThreadStart(Run));
            StreamIOThread.Start();
           // StreamIOThread.Interrupt();
            
        }
        

        public void Run()
        {
            LogClass.LogWrite("RawDataReceiveClass Run!"+Sock.Handle.ToString());
            int rSize;
            int cnt = 0;
            ptr = (IntPtr)buffer[0];
            try
            {
                StreamIOThread.Suspend();
            }
            catch(Exception e)
            {
                Console.WriteLine(e.HelpLink);
            }
            StreamIOThread.Resume();
            while ((rSize = Sock.Receive(buffer))>0)
            {
                LogClass.LogWrite(Sock.Handle.ToString()+"  "+(cnt++)+" "+rSize.ToString());
            }
            return;
        }
        
    }
}
