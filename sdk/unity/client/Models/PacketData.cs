namespace Client.Models
{
    public struct PacketData
    {
        public int ownerId;
        public int dataType;
        public int timeStamp;
        public int dataSize;
        public byte[] data;
    }
}