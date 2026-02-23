using Client;

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

    public void Run()
    {
        pigeonClient.ConnectAsync();
        while (pigeonClient.Connected)
        {
            
        }
    }

    private void SendData()
    {
        var data = Console.ReadLine();
        
    }

    private void ReceiveDatA()
    {
        
    }
}