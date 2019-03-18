using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WebApp.Models.Native
{
    public class CustomIOBufferClass
    {
        
        private int BufferSize { get; }
        private int SeekPtr { get; set; }
        private List<byte[]> BufferList;
        private byte[] MainBuffer { get; set; } = null;
        private byte[] BackBuffer { get; set; } = null;
        CustomIOBufferClass(int BufferCount = 2, int BufferSize = 32 * 1024 * 1024)
        {
            this.BufferSize = BufferSize;
            BufferList = new List<byte[]>();
            for(int i = 0; i<BufferCount; i++)
            {
                BufferList.Add(new byte[BufferSize]);
            }
            MainBuffer = BufferList[0];
            
        }
        public bool IsBufferAvailable()
        {
            return this.SeekPtr >= BufferSize ? true : false;
        }

        public int SwapBuffer()
        {
            SeekPtr = 0;
            byte[] temp;
            if (BackBuffer != null)
                temp = BackBuffer;
            BackBuffer = MainBuffer;
            MainBuffer
            

            return 1;
        }
        
    }
}
