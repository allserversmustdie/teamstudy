using System;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Collections.Concurrent;

namespace DummyClient_CS
{
    public partial class MainForm : Form
    {
        SocketWrapper socketWrapper = null;
        ConcurrentQueue<string> logQ = new ConcurrentQueue<string>();
        System.Windows.Forms.Timer mainTimer = new System.Windows.Forms.Timer();
        Thread receiveThread = null;
        string server_ip = "";
        int server_port = 0;


        public MainForm()
        {
            InitializeComponent();

            mainTimer.Interval = 100;
            mainTimer.Tick += new EventHandler(TimerHandler);
            mainTimer.Start();
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            server_ip = IPTextBox.Text;
            server_port = Int32.Parse(PortTextBox.Text);

            socketWrapper = new SocketWrapper();
            socketWrapper.Connect(server_ip, server_port);

            Log("접속시도 " + String.Format("IP: {0}, Port: {1}", server_ip, server_port));

            receiveThread = new Thread(Receive);
            receiveThread.Start();
        }

        private void LoginButton_Click(object sender, EventArgs e)
        {
            string userid = "goobobsa";
            PacketSender.Login(socketWrapper, userid);
            Log("로그인 패킷 보냄 user_id: " + userid);
        }

        private void LogoutButton_Click(object sender, EventArgs e)
        {

        }

        private void DisconnectButton_Click(object sender, EventArgs e)
        {
            socketWrapper.Disconnect();
            Log("접속종료");
        }

        public void Log(String log)
        {
            logQ.Enqueue(log);            
        }

        private void TimerHandler(object sender, EventArgs e)
        {
            string log;
            if (logQ.TryDequeue(out log))
            {
                LogBox.Items.Add(log);
            }            
        }

        public void Receive()
        {
            byte[] buffer = new byte[1024];
            int recvSize = 0;

            while (true)
            {
                if (socketWrapper != null && socketWrapper.IsConnected() == true)
                {
                    socketWrapper.Receive(ref buffer, ref recvSize);

                    if (recvSize == 0)
                    {
                        socketWrapper.Disconnect();
                    }
                    else if (recvSize > 0)
                    {
                        Packet packet = new Packet(ref buffer, recvSize);

                        DispatchPacket(ref packet);
                    }

                    recvSize = 0;
                    Array.Clear(buffer, 0, buffer.Length);
                }
            }

        }

        private void DispatchPacket(ref Packet packet)
        {
            PacketHeader header;

            packet.GetShort(out header.verfier);
            packet.GetInt(out header.length);
            packet.GetShort(out header.packetNo);

            if (header.verfier != 0xCFDA)
            {
                Log("Verifier가 다름니다");
                return;
            }

            switch (header.packetNo)
            {
                case 0:
                    {
                        
                    }
                    break;

                case 1:
                    {

                    }
                    break;

                case 2:
                    {
                        PacketDispatcher.Dispatcher_LoginResponse(ref packet);
                    }
                    break;

                case 3:
                    {

                    }
                    break;
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            mainTimer.Stop();

            if (receiveThread != null)
                receiveThread.Join();            

            if (socketWrapper != null)
                socketWrapper.Disconnect();
        }
    }
}
