using Client.Interfaces;

namespace ConsoleTest;

public class Logger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine(message);
    }
}