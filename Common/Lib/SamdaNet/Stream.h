#pragma once

#include "MemoryPool.h"
#include "PacketHeader.h"
#include "InterMsgHeader.h"

class ByteBuffer;

class Stream
{
protected:
	ByteBuffer* byteBuffer;
	unsigned int callerID;

public:
	Stream();
	Stream(ByteBuffer* buffer);
	
	void Clear();

	ByteBuffer* GetByteBuffer();
	char* GetReadBuffer();
	char* GetWriteBuffer();
	
	unsigned int GetCallerID();
	void SetCallerID(unsigned int id);

	bool GetByte(char& o);
	bool GetByte(unsigned char& o);
	bool GetShort(short& o);
	bool GetShort(unsigned short& o);
	bool GetInt(int& o);
	bool GetInt(unsigned int& o);
	bool GetInt64(long long& o);
	bool GetInt64(unsigned long long& o);
	bool GetFloat(float& o);
	bool GetDouble(double& o);
	bool GetBinary(char* out, int size);
	int GetString(char* out);
	int GetString(wchar_t* out);

	bool PutByte(char v);
	bool PutByte(unsigned char v);
	bool PutShort(short v);
	bool PutShort(unsigned short v);
	bool PutInt(int v);
	bool PutInt(unsigned int v);
	bool PutInt64(long long v);
	bool PutInt64(unsigned long long v);
	bool PutFloat(float v);
	bool PutDouble(double v);
	bool PutBinary(char* v, int size);
	bool PutString(char* v, int size);
	bool PutString(wchar_t* v, int size);
};

class Packet 
	: public Stream
	, public MemoryPool<Packet, PACKET_POOL_INIT_COUNT>
{
public:
	Packet();
	~Packet();

	PacketHeader* GetHeader(); // ����� �о�� �뵵, ����(��������)�� ��Ŷ�� ������ ��Ŷ����� �ѱ�� �뵵�� ���, ������ ���� ��ġ �̵�
	PacketHeader* GetHeaderPtr();  // ������ �����͸� ����� ��ȯ�Ͽ� ��ȯ, ������ ���� ��ġ�� �ű��� ����

	void MakePacket(unsigned short packetNo);
	void EndPacket();
};

class InterMsg 
	: public Stream
	, public MemoryPool<InterMsg, INTERMSG_POOL_INIT_COUNT>
{
public:
	InterMsg();
	~InterMsg();

	InterMsgHeader* GetHeaderPtr();  // ������ �����͸� ����� ��ȯ�Ͽ� ��ȯ, ������ ���� ��ġ�� �ű��� ����

	void MakeMessage(unsigned short packetNo);
	void EndMessage();
};