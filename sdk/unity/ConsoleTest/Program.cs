// See https://aka.ms/new-console-template for more information

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using Client;
using Client.Models;
using ConsoleTest;

Console.WriteLine("Hello, World!");

PigeonClient pigeonClient = new PigeonClient(new ClientOptions()
{
    Logger = new Logger(),
    Host = "127.0.0.1",
    Port = 12345,
});


await pigeonClient.ConnectAsync();
_ = Task.Run(pigeonClient.ReceiveAsync);
_ = Task.Run(pigeonClient.SendAsync);

void TrySend()
{
    Console.Write("Enter some data: ");
    var msg = Console.ReadLine();
    var packetData = new PacketData()
    {
        dataType = 1,
        ownerId = 1,
        timeStamp = 1,
    };
    packetData.SetPayload(msg);
    var size = Unsafe.SizeOf<PacketData>();
    byte[] buffer = new byte[size];
    Span<byte> destination = buffer;
    MemoryMarshal.Write(destination, packetData);

    var packetHeader = new PacketHeader()
    {
        packetType = 0,
    };
    size = Unsafe.SizeOf<PacketHeader>();
    byte[] buffer2 = new byte[size];
    Span<byte> destinationHeader = buffer2;
    MemoryMarshal.Write(destinationHeader, packetHeader);

    Span<byte> merged = new byte[destinationHeader.Length + destination.Length];
    destinationHeader.CopyTo(merged);
    destination.CopyTo(merged.Slice(destinationHeader.Length));

    pigeonClient.QueueData(merged.ToArray());
}

void TryReceive()
{
    var data = pigeonClient.GetData();
    if (data == null || data.Length == 0)
    {
        Console.WriteLine("Data is empty");
        return;
    }
    var packetHeaderSize = Unsafe.SizeOf<PacketHeader>();
    var packetHeader = new ArraySegment<byte>(data,0, packetHeaderSize);
    var packetDataSize = Unsafe.SizeOf<PacketData>();
    var packetData =  new ArraySegment<byte>(data, packetHeaderSize, packetDataSize);
    
    var packetHeaderStruct =MemoryMarshal.Read<PacketHeader>(packetHeader);
    var packetDataStruct =MemoryMarshal.Read<PacketData>(packetData);
    var msg = packetDataStruct.GetPayload();
    Console.WriteLine(msg);
}

while (pigeonClient.Connected)
{
    TrySend();
    TryReceive();
}