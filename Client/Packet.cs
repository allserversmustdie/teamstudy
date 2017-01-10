using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DummyClient_CS
{
    struct PacketHeader
    {
        public ushort verfier;
        public uint length;
        public ushort packetNo;

        public PacketHeader(ushort pktNo)
        {
            verfier = 0xCFDA;
            length = 0;
            packetNo = pktNo;            
        }
    }

    class Packet
    {
        private const int MAX_PACKET_SIZE = 200000;
        public byte[] buffer;
        public int index;
        public int length;

        public Packet()
        {
            buffer = new byte[MAX_PACKET_SIZE];
            index = 0;
            length = 0;
        }

        public Packet(ref byte[] rawData, int size)
        {
            buffer = new byte[size];
            Array.Copy(rawData, buffer, size);
            index = 0;
            length = size;
        }

        public void MakePacket(ushort packetNo)
        {
            PacketHeader header = new PacketHeader(packetNo);
            // verfier 필드 넣기
            WriteShort(header.verfier);
            // length 필드 넣기
            WriteInt(header.length);
            // 패킷번호 필드 넣기
            WriteShort(header.packetNo);
        }

        public void SetLength(int length)
        {
            // 2 => verfier 다음 위치
            Array.Copy(BitConverter.GetBytes(length), 0, buffer, 2, sizeof(int));
            index = length;
        }

        public int GetLength()
        {
            return index;
        }

        public void EndPacket()
        {
            SetLength(index);
        }

        public bool WriteByte(byte v)
        {
            if (index + sizeof(byte) >= MAX_PACKET_SIZE)
                return false;

            // source array, source index, dest array, dest index, size
            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(byte));
            index += sizeof(byte);

            return true;
        }

        public bool WriteByte(sbyte v)
        {
            if (index + sizeof(sbyte) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(sbyte));
            index += sizeof(sbyte);

            return true;
        }

        public bool WriteShort(short v)
        {
            if (index + sizeof(short) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(short));
            index += sizeof(short);

            return true;
        }

        public bool WriteShort(ushort v)
        {
            if (index + sizeof(ushort) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(ushort));
            index += sizeof(ushort);

            return true;
        }

        public bool WriteInt(int v)
        {
            if (index + sizeof(int) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(int));
            index += sizeof(int);

            return true;
        }

        public bool WriteInt(uint v)
        {
            if (index + sizeof(uint) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(uint));
            index += sizeof(uint);

            return true;
        }

        public bool WriteFloat(float v)
        {
            if (index + sizeof(float) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(float));
            index += sizeof(float);

            return true;
        }

        public bool WriteLong(long v)
        {
            if (index + sizeof(long) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(long));
            index += sizeof(long);

            return true;
        }

        public bool WriteLong(ulong v)
        {
            if (index + sizeof(ulong) >= MAX_PACKET_SIZE)
                return false;

            Array.Copy(BitConverter.GetBytes(v), 0, buffer, index, sizeof(ulong));
            index += sizeof(ulong);

            return true;
        }

        public bool WriteString(string v)
        {
            int stringSize = v.Length;
            if (index + stringSize >= MAX_PACKET_SIZE)
                return false;

            WriteInt(stringSize);

            Array.Copy(Encoding.ASCII.GetBytes(v), 0, buffer, index, stringSize);
            index += stringSize;

            return true;
        }

        public bool WriteArray(byte[] v)
        {
            int arraySize = v.Length;
            if (index + arraySize >= MAX_PACKET_SIZE)
                return false;

            WriteInt(arraySize);

            Array.Copy(v, 0, buffer, index, arraySize);
            index += arraySize;

            return true;
        }

        // Get Methods
        public bool GetByte(out byte v)
        {
            v = Convert.ToByte(BitConverter.ToChar(buffer, index));
            index += sizeof(byte);

            return true;
        }

        public bool GetByte(out sbyte v)
        {
            v = Convert.ToSByte(BitConverter.ToChar(buffer, index));
            index += sizeof(sbyte);

            return true;
        }

        public bool GetShort(out short v)
        {
            v = BitConverter.ToInt16(buffer, index);
            index += sizeof(short);

            return true;
        }

        public bool GetShort(out ushort v)
        {
            v = BitConverter.ToUInt16(buffer, index);
            index += sizeof(ushort);

            return true;
        }

        public bool GetInt(out int v)
        {
            v = BitConverter.ToInt32(buffer, index);
            index += sizeof(int);

            return true;
        }

        public bool GetInt(out uint v)
        {
            v = BitConverter.ToUInt32(buffer, index);
            index += sizeof(uint);

            return true;
        }

        public bool GetFloat(out float v)
        {
            v = BitConverter.ToSingle(buffer, index);
            index += sizeof(float);

            return true;
        }

        public bool GetLong(out long v)
        {
            v = BitConverter.ToInt64(buffer, index);
            index += sizeof(long);

            return true;
        }

        public bool GetLong(out ulong v)
        {
            v = BitConverter.ToUInt64(buffer, index);
            index += sizeof(ulong);

            return true;
        }

        public bool GetString(out string v)
        {
            int stringSize = 0;
            GetInt(out stringSize);

            v = Encoding.ASCII.GetString(buffer, index, stringSize);
            index += stringSize;

            return true;
        }

        public bool GetArray(ref byte[] v, int readSize)
        {
            Array.Copy(buffer, index, v, 0, readSize);
            index += readSize;

            return true;
        }
    }
}
