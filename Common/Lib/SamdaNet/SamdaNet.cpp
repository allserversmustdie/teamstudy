#include "Precompiled.h"
#include "SamdaNet.h"
#include "Dispatcher.h"
#include "Socket.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"
#include "DBManager.h"
#include "NetworkThread.h"
#include "MainThread.h"
#include "DBThread.h"
#include "Dispatcher.h"
#include "TimerQueue.h"
#include "Acceptor.h"
#include "AcceptorManager.h"

SamdaNet::SamdaNet()
	: ioMultiplexer(nullptr)
	, sessionManager(nullptr)
	, dbManager(nullptr)
	, networkThread(nullptr)
	, mainThread(nullptr)
	, dbThread(nullptr)
	, acceptorManager(nullptr)
	, messageQueue(nullptr)
	, timerQueue(nullptr)
	, initialized(false) 
	, running(false)
{
	Init();
}

SamdaNet::~SamdaNet()
{
	Stop();

	Close();
}

bool SamdaNet::Init()
{
	if (initialized == true)
		return false;

#ifdef _WINDOWS
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		return false;
#endif 

	// DBó���� ����� DB ������ ����
	dbThread = new DBThread(*this);

	// IO�� ����� ��Ʈ��ũ ������ ���� (Worker Thread)
	networkThread = new NetworkThread(*this);

	// ��Ŷ�� ó���� ���ν�����(Logic Thread) ����
	mainThread = new MainThread(*this);

	// IO��ü ����
	ioMultiplexer = new IOMultiplexer();
	if (ioMultiplexer->create() == false)
		return false;

	// DB������ �����ϴ� �Ŵ��� ����
	dbManager = new DBManager;
	
	// ����� ���ǵ��� �����ϱ� ���� ���� �Ŵ��� ����
	sessionManager = new SessionManager(*this);

	// �������� port�� �������� accept �Ŵ��� ���� (�Ϲ� ������ 1���� ���)
	acceptorManager = new AcceptorManager(ioMultiplexer, sessionManager);

	// ��Ŷ�� ��Ƴ��� ť �Ŵ��� ����
	messageQueue = new MessageQueue();

	// Ÿ�̸Ӹ� ��Ƴ��� ť ����
	timerQueue = new TimerQueue();

	// �ʱ�ȭ �Ϸ�
	initialized = true;

	return true;
}

void SamdaNet::Close()
{
	SAFE_DELETE(dbManager);
	SAFE_DELETE(acceptorManager);
	SAFE_DELETE(dbThread);
	SAFE_DELETE(mainThread);
	SAFE_DELETE(networkThread);
	SAFE_DELETE(sessionManager);
	SAFE_DELETE(messageQueue);
	SAFE_DELETE(timerQueue);
	SAFE_DELETE(ioMultiplexer);

#ifdef _WINDOWS
	WSACleanup();
#endif
}

SessionManager* SamdaNet::GetSessionManager()
{
	return sessionManager;
}

MessageQueue* SamdaNet::GetMessageQueue()
{
	return messageQueue;
}

TimerQueue* SamdaNet::GetTimerQueue()
{
	return timerQueue;
}

IOMultiplexer* SamdaNet::GetIOMultiplexer()
{
	return ioMultiplexer;
}

DBManager* SamdaNet::GetDBManager()
{
	return dbManager;
}

bool SamdaNet::Start()
{
	// �����÷��׸� �Ѱ�, �ʿ��� �����带 ���۽�Ų��
	SetRunning(true);

	mainThread->Start();

	networkThread->Start();
	
	dbThread->Start();

	return true;
}

void SamdaNet::Stop()
{
	networkThread->Stop();

	mainThread->Stop();

	// �����÷��� OFF
	SetRunning(false);
}

void SamdaNet::SetRunning(bool flag)
{
	LockGuard<Mutex> lock(runningSync);

	running = flag;
}

bool SamdaNet::IsRunning()
{
	LockGuard<Mutex> lock(runningSync);

	return running;
}

void SamdaNet::SetDBDispatcher(IDBDispatcher* disp)
{
	dbThread->SetDBDispatcher(disp);
}

void SamdaNet::SetInterDispatcher(IInterDispatcher* disp)
{
	mainThread->SetInterDispatcher(disp);
}

void SamdaNet::SetTimerDispatcher(ITimerDispatcher* disp)
{
	mainThread->SetTimerDispatcher(disp);
}

bool SamdaNet::Open(const wchar_t* ip, unsigned short port, IDispatcher* disp)
{
	Acceptor* acceptor = acceptorManager->create(disp);
	if (acceptor->Open(ip, port) == false)
		return false;

	return true;
}

bool SamdaNet::Connect(const wchar_t* ip, unsigned short port, IDispatcher* disp)
{
	return true;
}


