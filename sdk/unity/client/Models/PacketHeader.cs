namespace Client.Models
{
    public struct PacketHeader
    {
        public byte packetType;
        public int bodySize;
    }
}