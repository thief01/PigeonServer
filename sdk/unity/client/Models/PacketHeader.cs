using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Client.Models
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct PacketHeader
    {
        public byte packetType;
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public unsafe struct PacketData
    {
        private const int DATA_SIZE = 256;
        public int ownerId;
        public int dataType;
        public int timeStamp;
        public fixed byte payload[DATA_SIZE];
        
        public unsafe void SetPayload(string text)
        {
            var bytes = System.Text.Encoding.UTF8.GetBytes(text);

            fixed (byte* ptr = payload)
            {
                for (int i = 0; i < 256; i++)
                    ptr[i] = 0;

                int length = Math.Min(bytes.Length, 256);
                for (int i = 0; i < length; i++)
                    ptr[i] = bytes[i];
            }
        }
        
        public unsafe string GetPayload()
        {
            Console.WriteLine($"dataSize: {DATA_SIZE}"); // debug
    
            if (DATA_SIZE <= 0 || DATA_SIZE > DATA_SIZE)
                throw new Exception($"Invalid dataSize: {DATA_SIZE}");
        
            fixed (byte* ptr = payload)
            {
                return Encoding.UTF8.GetString(ptr, DATA_SIZE);
            }
        }
    }
}