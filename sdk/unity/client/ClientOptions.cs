using Client.Interfaces;

namespace Client
{
    public class ClientOptions
    {
        public string Host { get; set; } = "127.0.0.1";
        public int Port { get; set; } = 12345;
        public ILogger? Logger { get; set; }
        public int TimeoutMs { get; set; } = 5000;
    }
}