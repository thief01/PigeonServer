using Client;
using Client.Models;

namespace ConsoleTest;

public class PigeonApp
{
    private PigeonClient pigeonClient;

    public PigeonApp()
    {
        pigeonClient = new PigeonClient(new ClientOptions()
        {
            Logger = new Logger(),
            Host = "127.0.0.1",
            Port = 12345,
        });
    }

    public async Task Run()
    {
        await pigeonClient.ConnectAsync();
        _ = pigeonClient.SendAsync();
        _ = pigeonClient.ReceiveAsync();
        while (pigeonClient.Connected)
        {
            SendData();
            ReceiveData();
        }
    }

    private void SendData()
    {
        var text = Console.ReadLine();
        if (string.IsNullOrEmpty(text))
            return;

        var header = new PacketHeader()
        {
            packetType = 0x01
        };

        var packet = new PacketData()
        {
            ownerId = 1,
            dataType = 1,
            timeStamp = (int)DateTimeOffset.UtcNow.ToUnixTimeSeconds()
        };

        packet.SetPayload(text);
        
        byte[] headerBytes = StructSerializer.ToBytes(header);
        byte[] packetBytes = StructSerializer.ToBytes(packet);
        
        byte[] finalPacket = new byte[headerBytes.Length + packetBytes.Length];

        Buffer.BlockCopy(headerBytes, 0, finalPacket, 0, headerBytes.Length);
        Buffer.BlockCopy(packetBytes, 0, finalPacket, headerBytes.Length, packetBytes.Length);

        pigeonClient.QueueData(finalPacket);
    }

    private void ReceiveData()
    {
        var data = pigeonClient.GetData(); 
        if (data.Length == 0)
            return;

        // --- DESERIALIZACJA ---
        const int HEADER_SIZE = 1;
        const int DATA_PACKET_SIZE = 268;

        byte packetType = data[0];
        
        byte[] dataBytes = new byte[DATA_PACKET_SIZE];
        Array.Copy(data, HEADER_SIZE, dataBytes, 0, DATA_PACKET_SIZE);

        int ownerId = BitConverter.ToInt32(dataBytes, 0);
        int dataType = BitConverter.ToInt32(dataBytes, 4);
        int timeStamp = BitConverter.ToInt32(dataBytes, 8);

        byte[] payloadBytes = new byte[256];
        Array.Copy(dataBytes, 12, payloadBytes, 0, 256);

        string payloadStr = System.Text.Encoding.UTF8.GetString(payloadBytes).TrimEnd('\0');
        
        Console.WriteLine($"[RECEIVE] PacketType: {packetType}, OwnerId: {ownerId}, DataType: {dataType}, TimeStamp: {timeStamp}, Payload: '{payloadStr}'");
    }
}