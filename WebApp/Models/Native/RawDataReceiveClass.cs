using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace WebApp.Models.Native
{
   
    public class RawDataReceiveClass 
    {
        Socket Sock;
        Thread StreamIOThread;
        byte[] buffer;
        public RawDataReceiveClass(Socket sock)
        {
            this.Sock = sock;
            buffer = new byte[32 * 1024 * 1024];
            Thread StreamIOThread = new Thread(new ThreadStart(Run));
            StreamIOThread.Start();
        }

        public void Run()
        {
            int rSize;
            while ((rSize = Sock.Receive(buffer))>0)
            {
                Console.Write(buffer.ToString());
            }
            return;
        }
    }
}
