using System;
using System.Net.Sockets;

namespace Client
{
    public class Client
    {
        public Action OnConnected;
        public TcpClient TcpClient { get; private set; }

        public Client()
        {
            TcpClient = new TcpClient();
        }

        public void Connect(string host, int port)
        {
            TcpClient.Connect(host, port);
        }
    }
}