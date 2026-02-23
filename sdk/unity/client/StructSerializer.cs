using System;
using System.Runtime.InteropServices;

namespace Client
{
    public class StructSerializer
    {
        public static byte[] ToBytes<T>(T str) where T : struct
        {
            int size = Marshal.SizeOf<T>();
            byte[] arr = new byte[size];

            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(str, ptr, true);
                Marshal.Copy(ptr, arr, 0, size);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }

            return arr;
        }
    }
}