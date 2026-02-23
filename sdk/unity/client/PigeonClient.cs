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
            LogWrapper("Sending loop");
            var stream = tcpClient.GetStream();
            while (tcpClient.Connected)
            {
                if (sendQueue.TryDequeue(out byte[] data))
                {
                    await stream.WriteAsync(data, 0,data.Length);
                    LogWrapper($"Sent: {data.Length}");
                }
                else
                {
                    await Task.Delay(1);
                }
            }
        }

        public async Task ReceiveAsync()
        {
            LogWrapper("Receiving loop");
            var stream = tcpClient.GetStream();

            const int HEADER_SIZE = 1;
            const int DATA_PACKET_SIZE = 268; // ownerId + dataType + timestamp + payload
            const int TOTAL_SIZE = HEADER_SIZE + DATA_PACKET_SIZE;

            byte[] buffer = new byte[TOTAL_SIZE];

            while (tcpClient.Connected)
            {
                int received = 0;

                // odbierz dokładnie TOTAL_SIZE bajtów
                while (received < TOTAL_SIZE)
                {
                    int n = await stream.ReadAsync(buffer, received, TOTAL_SIZE - received);
                    if (n == 0)
                    {
                        LogWrapper("Disconnected");
                        return;
                    }
                    received += n;
                }

                // teraz mamy cały pakiet w buffer
                byte[] packetCopy = new byte[TOTAL_SIZE];
                Array.Copy(buffer, packetCopy, TOTAL_SIZE);

                receiveQueue.Enqueue(packetCopy);
                LogWrapper("Received full packet: " + TOTAL_SIZE);
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