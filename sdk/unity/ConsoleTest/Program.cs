// See https://aka.ms/new-console-template for more information

using Client;
using ConsoleTest;

Console.WriteLine("Hello, World!");

PigeonClient pigeonClient = new PigeonClient(new ClientOptions()
{
    Logger = new Logger(),
    Host = "127.0.0.1",
    Port = 12345,
});


await pigeonClient.ConnectAsync();
await pigeonClient.ReceiveAsync();