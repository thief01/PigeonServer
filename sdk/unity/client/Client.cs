using System;
using System.Net.Sockets;
using System.Threading.Tasks;
using Client.Interfaces;

namespace Client
{
    public class Client
    {
        public Action OnConnected;
        public TcpClient TcpClient { get; private set; }

        private ClientOptions options;
        private ILogger logger;

        public Client(ClientOptions options)
        {
            this.options = options;
            logger = options.Logger;
            TcpClient = new TcpClient();
        }
        
        public void Disconnect()
        {
            TcpClient.Close();
        }

        public async Task ConnectAsync()
        {
            try
            {
                await TcpClient.ConnectAsync(options.Host, options.Port);
                if (TcpClient.Connected)
                {
                    options.Logger?.Log($"Connected to {options.Host}:{options.Port}");
                    OnConnected?.Invoke();
                }
            }
            catch (Exception e)
            {
                options.Logger?.Log(e.ToString());
            }
        }
    }
}