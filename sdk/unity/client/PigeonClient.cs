using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Client.Interfaces;
using Client.Models;
using Newtonsoft.Json;

namespace Client
{
    public class PigeonClient
    {
        public bool Connected => tcpClient.Connected;
        public Action OnConnected;
        
        private ConcurrentQueue<byte[]> sendQueue = new ConcurrentQueue<byte[]>();
        private ConcurrentQueue<byte[]> receiveQueue = new ConcurrentQueue<byte[]>();
        
        private TcpClient tcpClient;
        private ClientOptions options;
        private ILogger? logger;

        public PigeonClient(ClientOptions options)
        {
            this.options = options;
            logger = options.Logger;
            tcpClient = new TcpClient();
        }
        
        public async Task ConnectAsync()
        {
            try
            {
                await tcpClient.ConnectAsync(options.Host, options.Port);
                if (tcpClient.Connected)
                {
                    LogWrapper($"Connected to {options.Host}:{options.Port}");
                    OnConnected?.Invoke();
                }
                else
                {
                    LogWrapper("Couldn't connect");
                }
            }
            catch (Exception e)
            {
                LogWrapper(e.ToString());
            }
        }

        public async Task SendAsync()
        {
            var stream = tcpClient.GetStream();
            while (tcpClient.Connected)
            {
                if (sendQueue.TryDequeue(out byte[] data))
                {
                    await stream.WriteAsync(data, 0,data.Length);
                    LogWrapper($"Sent: {data.Length}");
                }
            }
        }

        public async Task ReceiveAsync()
        {
            var stream = tcpClient.GetStream();
            var buffer = new byte[1024];

            while (tcpClient.Connected)
            {
                int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0)
                {
                    LogWrapper("Disconnected");
                    break;
                }
                
                LogWrapper("Received: " + bytesRead);
                receiveQueue.Enqueue(buffer);
            }
        }
        
        public void Disconnect()
        {
            tcpClient.Close();
        }

        public void QueueData(byte[] data)
        {
            sendQueue.Enqueue(data);
        }

        public byte[] GetData()
        {
            if (receiveQueue.TryDequeue(out var data))
            {
                return data;
            }

            return Array.Empty<byte>();
        }

        private void LogWrapper(string log)
        {
            logger?.Log(log);
        }
    }
}