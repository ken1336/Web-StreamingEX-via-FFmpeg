using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace WebApp.Models.Native
{
    public class CustomIOBufferClass
    {


        private int BufferSize { get; }
        private int SeekPtr;
        private List<byte[]> BufferList;
        private unsafe byte[] MainBuffer { get; set; } = null;
        private unsafe byte[] BackBuffer { get; set; } = null;

        [StructLayout(LayoutKind.Sequential)]
        public struct CoustomIOBuffer
        {
            [MarshalAs(UnmanagedType.I4)]
            unsafe int* BufferPTR;
            [MarshalAs(UnmanagedType.IUnknown)]
            unsafe byte** BufferRef;

            public unsafe void Swap(int* ptr, byte* buffer)
            {

                this.BufferPTR = ptr;
                this.BufferRef = &buffer;
            }
        }
        CoustomIOBuffer CoustomBuffer;





        [DllImport("ows.dll", EntryPoint = "swap_buffer", CallingConvention = CallingConvention.StdCall)]
        public static extern int StartModule(out CoustomIOBuffer buffer);

        unsafe CustomIOBufferClass(int BufferCount = 2, int BufferSize = 32 * 1024 * 1024)
        {
            MainBuffer = new byte[32 * 1024 * 1024];
            BackBuffer = new byte[32 * 1024 * 1024];
            
            
            fixed(byte* pointer = BackBuffer)
            {

                fixed(int* ptr = &SeekPtr)
                {
                    CoustomBuffer.Swap(ptr, pointer);
                }
               
               
                
            }
            
           

        }
        public bool IsBufferAvailable()
        {
            return this.SeekPtr >= BufferSize ? true : false;
        }

        public int SwapBuffer()
        {
            SeekPtr = 0;
            byte[] temp = null;
            if (BackBuffer != null)
                temp = BackBuffer;
            BackBuffer = MainBuffer;
            MainBuffer = temp;
            

            return 1;
        }
        
    }
}
