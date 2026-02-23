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
        
        public unsafe void SetPayload(string message)
        {
            var bytes = Encoding.UTF8.GetBytes(message);

            if (bytes.Length > DATA_SIZE)
                throw new Exception("Payload too big");
            

            fixed (byte* dst = payload)
            fixed (byte* src = bytes)
            {
                Buffer.MemoryCopy(src, dst, DATA_SIZE, bytes.Length);
            }
        }
        
        public unsafe string GetPayload()
        {
            Console.WriteLine($"dataSize: {DATA_SIZE}"); // debug
    
            if (DATA_SIZE <= 0 || DATA_SIZE > 255)
                throw new Exception($"Invalid dataSize: {DATA_SIZE}");
        
            fixed (byte* ptr = payload)
            {
                return Encoding.UTF8.GetString(ptr, DATA_SIZE);
            }
        }
    }
}