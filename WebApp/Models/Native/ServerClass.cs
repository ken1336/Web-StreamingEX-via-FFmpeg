using System;
using System.Collections.Generic;
using System.Linq;

using System.Threading.Tasks;
using System.Net;

using System.Net.Sockets;
using System.Collections.Generic;
using System.Threading;


namespace WebApp.Models.Native
{
    public class ServerClass
    {
        Dictionary<int, RawDataReceiveClass> SocketMap;
        TcpListener Server;
        
        Socket Client;
        int ClientNum = 0;
        public ServerClass()
        {
            LogClass.LogWrite("hello this is test!");
            Thread BindingThread = new Thread(new ThreadStart(Run));
            BindingThread.Start();
        }
        void Run()
        {
            Server = new TcpListener(8889);
            Server.Start();

            while (true)
            {
                try
                {
                    ClientNum++;
                    Socket sock = Server.AcceptSocket();
                    
                    SocketMap.Add(ClientNum, new RawDataReceiveClass(sock));
                }
                catch (Exception e)
                {
                    LogClass.LogWrite(e.Message);
                }
            }

        }

        

    }
}
