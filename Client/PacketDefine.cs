using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient_CS
{
    struct PacketHeader
    {
        public uint length;
        public ushort packetNo;

        public PacketHeader(ushort pktNo)
        {
            length = 0;
            packetNo = pktNo;
        }
    }

    [StructLayout(LayoutKind.Sequential, Pack=1)]
    struct PacketDefine
    {
    }
}
