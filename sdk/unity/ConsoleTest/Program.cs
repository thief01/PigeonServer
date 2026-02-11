// See https://aka.ms/new-console-template for more information

using Client;
using ConsoleTest;

Console.WriteLine("Hello, World!");

Client.Client client = new Client.Client(new ClientOptions()
{
    Logger = new Logger(),
    Host = "127.0.0.1",
    Port = 12345,
});

await client.ConnectAsync();