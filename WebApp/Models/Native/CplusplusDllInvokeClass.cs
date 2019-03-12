using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Net;

using System.Net.Sockets;
using System.Collections.Generic;



namespace WebApp.Models
{
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public unsafe class CplusplusDllInvokeClass
    {
       

        CplusplusDllInvokeClass()
        {
            
            //Server = new TcpListener(8889);
            //Server.Start();

            //while (true)
            //{
            //    try
            //    {
            //        ClientNum++;
            //        Socket sock = Server.AcceptSocket();
                   
            //        SocketMap.Add(ClientNum, sock);
            //    }catch(Exception e)
            //    {
            //        Console.Write(e.Message);
            //    }
            //}
            
        }
        ~CplusplusDllInvokeClass()
        {

        }

    }
}
