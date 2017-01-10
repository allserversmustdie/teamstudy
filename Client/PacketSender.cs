using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient_CS
{
    class PacketSender
    {
        public static void Login(SocketWrapper s, string userid)
        {
            Packet packet = new Packet();

            packet.MakePacket(1);
            packet.WriteString(userid);

            s.Send(ref packet);
        }
    }
}
