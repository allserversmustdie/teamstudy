using System;
using System.Net;
using System.Net.Sockets;

namespace DummyClient_CS
{
    class SocketWrapper
    {
        Socket socket       = null;
        IPAddress serverIp  = null;
        int serverPort      = 0;

        public void Connect(string ip, int port)
        {
            serverIp    = IPAddress.Parse(ip);
            serverPort  = port;

            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.Connect(serverIp, serverPort);
        }

        public void Disconnect()
        {
            if (socket.Connected)
            {
                socket.Disconnect(true);
                socket.Close();
            }
        }

        public bool IsConnected()
        {
            return socket.Connected;
        }

        public void Send(ref Packet packet)
        {
            packet.EndPacket();

            try
            {
                int total = 0;
                int size = packet.GetLength();
                int left_data = size;
                int send_data = 0;

                while (total < size)
                {
                    send_data = socket.Send(packet.buffer, left_data, SocketFlags.None);
                    total += send_data;
                    left_data -= send_data;
                }
            }
            catch (Exception e)
            {
                Program.mainForm.Log(e.Message);
            }
        }

        public void Receive(ref byte[] buffer, ref int receiveSize)
        {
            try
            {
                while (socket != null)
                {
                    int total = 0;
                    int size = 0;
                    int left_data = 0;
                    int recv_data = 0;

                    if (!socket.Connected)
                        break;

                    recv_data = socket.Receive(buffer, total, buffer.Length - total, SocketFlags.None);
                    total += recv_data;
                    if (total < 6)
                        continue;

                    size = BitConverter.ToInt32(buffer, 2);
                    left_data = size - total;

                    if (left_data == 0)
                    {
                        receiveSize = total;
                        break;
                    }

                    while (total < size)
                    {
                        recv_data = socket.Receive(buffer, total, left_data, SocketFlags.None);
                        if (recv_data == 0)
                        {
                            receiveSize = total;
                            break;
                        }
                        total += recv_data;
                        left_data -= recv_data;
                    }                    
                }
            }
            catch (Exception e)
            {
                //Program.mainForm.Log(e.Message);
            }
        }
/*
        private void Connect()
        {
            IPEndPoint ipep = new IPEndPoint(serverIp, serverPort);
            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.BeginConnect(ipep, new AsyncCallback(OnConnect), socket);
        }

        private void OnConnect(IAsyncResult ar)
        {
            try
            {
                socket = (Socket)ar.AsyncState;
                IPEndPoint serverIpep = (IPEndPoint)socket.RemoteEndPoint;
                socket.EndConnect(ar);
            }
            catch (SocketException e)
            {
                if (e.SocketErrorCode == SocketError.NotConnected)
                {

                }
            }
        }
        
        public void Send(ref Packet packet)
        {
            packet.EndPacket();
            this.Send(ref packet.buffer, packet.index);
        }

        public void Send(ref byte[] buffer, int size)
        {
            try
            {
                socket.BeginSend(buffer, 0, size, SocketFlags.None, new AsyncCallback(OnSent), socket);
            }
            catch (SocketException e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        public void OnSent(IAsyncResult ar)
        {
            try
            {
                socket = (Socket)ar.AsyncState;
                int bytesSent = socket.EndSend(ar);
            }
            catch (Exception e)
            {

            }
        }

        public int Receive(ref byte[] buffer)
        {
            return socket.BeginReceive(buffer, SocketFlags.None, new AsyncCallback(OnReceived), socket);
        }

        public void OnReceived(IAsyncResult ar)
        {
            try
            {
                socket = (Socket)ar.AsyncState;
                int bytesRead = socket.EndReceive(ar);
            }
            catch (Exception e)
            {

            }
        }
*/
    }
}
