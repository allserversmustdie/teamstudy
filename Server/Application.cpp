#include "Precompiled.h"
#include "Application.h"
#include "ConfigManager.h"
#include "ConfigReader.h"
#include "ClientDispatcher.h"
#include "DBDispatcher.h"
#include "InterDispatcher.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "TimerDispatcher.h"
#include "DBManager.h"
#include "SamdaNet.h"
#include "GameManager.h"
#include "Log.h"

Application::Application()
	: networkEngine(nullptr)
	, cfgManager(nullptr)
	, clientDispatcher(nullptr)
	, dbDispatcher(nullptr)
	, interDispatcher(nullptr)
	, timerDispatcher(nullptr)
	, isRunning(false)
{

}

Application::~Application()
{
	delete dbDispatcher;
	delete timerDispatcher;
	delete interDispatcher;
	delete clientDispatcher;
	delete networkEngine;
	delete cfgManager;
	delete gameManager;
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init(int argc, wchar_t* argv[])
{
	// 필요한 객체들 생성
	cfgManager = new ConfigManager;
	networkEngine = new SamdaNet;
	clientDispatcher = new ClientDispatcher;
	dbDispatcher = new DBDispatcher;
	interDispatcher = new InterDispatcher;
	timerDispatcher = new TimerDispatcher;
	gameManager = new GameManager;

	// 설정 cfg 읽기
	ReadConfig(argv[1]);

	// 네트워크 설정
	const wchar_t* server_ip = cfgManager->serverConfig.ip.c_str();
	unsigned int server_port = cfgManager->serverConfig.port;

	if (!networkEngine->Open(server_ip, server_port, clientDispatcher))
	{
		LOG_CRI(L"Fail to start NetworkEngine! ip: %s, port: %d", server_ip, server_port);
		return false;
	}

	// DB연결설정
	for (int i = ACCOUNT_DB; i < MAX_DB_HANDLE; ++i)
	{
		const wchar_t* db_ip = cfgManager->dbConfig[i].ip.c_str();
		unsigned int db_port = cfgManager->dbConfig[i].port;
		const wchar_t* db_name = cfgManager->dbConfig[i].dbname.c_str();
		const wchar_t* db_user = cfgManager->dbConfig[i].user.c_str();
		const wchar_t* db_passwd = cfgManager->dbConfig[i].passwd.c_str();

		networkEngine->GetDBManager()->Open(i, db_ip, db_port, db_name, db_user, db_passwd);
		networkEngine->SetDBDispatcher(dbDispatcher);
	}

	// InterDispatcher 설정
	networkEngine->SetInterDispatcher(interDispatcher);

	// TimerDispatcher 설정
	networkEngine->SetTimerDispatcher(timerDispatcher);

	// 게임매니저 설정
	gameManager->Init();

	LOG_INFO(L"Server started! ip: %s, port: %d", server_ip, server_port);

	SetRunningFlag(true);

	networkEngine->Start();	

	return true;
}

void Application::Process()
{
	while (isRunning)
	{
		GAME_MANAGER()->Init();

		getchar();
		isRunning = false;
		Sleep(1);
	}
}

void Application::Close()
{
	networkEngine->Stop();
}

void Application::SetRunningFlag(bool flag)
{
	isRunning = flag;
}

bool Application::IsRunning()
{
	return isRunning;
}

GameManager* Application::GetGameManager()
{
	return gameManager;
}

SamdaNet* Application::GetNetworkEngine()
{
	return networkEngine;
}

bool Application::ReadConfig(wchar_t* cfgname)
{
	ConfigReader servercfg;

	wstring cfgPath = L"cfg/";
	cfgPath.append(cfgname);

	servercfg.Open(cfgPath);
	cfgManager->serverConfig.ip = servercfg.GetString(L"ip");
	cfgManager->serverConfig.port = servercfg.GetInt(L"port");

	cfgManager->dbConfig[ACCOUNT_DB].ip = servercfg.GetString(L"account_db_ip");
	cfgManager->dbConfig[ACCOUNT_DB].port = servercfg.GetInt(L"account_db_port");
	cfgManager->dbConfig[ACCOUNT_DB].user = servercfg.GetString(L"account_db_user");
	cfgManager->dbConfig[ACCOUNT_DB].passwd = servercfg.GetString(L"account_db_passwd");
	cfgManager->dbConfig[ACCOUNT_DB].dbname = servercfg.GetString(L"account_db_name");

	cfgManager->dbConfig[GAME_DB].ip = servercfg.GetString(L"game_db_ip");
	cfgManager->dbConfig[GAME_DB].port = servercfg.GetInt(L"game_db_port");
	cfgManager->dbConfig[GAME_DB].user = servercfg.GetString(L"game_db_user");
	cfgManager->dbConfig[GAME_DB].passwd = servercfg.GetString(L"game_db_passwd");
	cfgManager->dbConfig[GAME_DB].dbname = servercfg.GetString(L"game_db_name");

	return true;
}
