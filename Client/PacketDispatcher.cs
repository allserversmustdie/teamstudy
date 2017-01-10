using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient_CS
{
    class PacketDispatcher
    {
        public static void Dispatcher_LoginResponse(ref Packet packet)
        {
            int result = 0;
            int m_id = 0;
            //string str = "";

            packet.GetInt(out result);
            packet.GetInt(out m_id);
            //packet.GetString(out str);

            if (result == 1)
            {
                Program.mainForm.Log("로그인 성공 m_id: " + m_id.ToString());
            }
            else
            {
                Program.mainForm.Log("로그인 실패");
            }
            
        }
    }
}
