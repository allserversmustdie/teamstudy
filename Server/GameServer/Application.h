#pragma once

#include "GlobalDefines.h"

class SamdaNet;
class ConfigManager;
class ClientDispatcher;
class DBDispatcher;
class InterDispatcher;
class TimerDispatcher;
class GameManager;

class Application
{
private:
	ConfigManager* cfgManager;
	SamdaNet* networkEngine;
	ClientDispatcher* clientDispatcher;
	DBDispatcher* dbDispatcher;
	InterDispatcher* interDispatcher;
	TimerDispatcher* timerDispatcher;
	GameManager* gameManager;
	bool isRunning;

public:
	Application();
	~Application();

	static Application& GetInstance();

	bool Init(int argc, wchar_t* argv[]);
	void Process();
	void Close();

	void SetRunningFlag(bool flag);
	bool IsRunning();


	GameManager* GetGameManager();
	SamdaNet* GetNetworkEngine();

private:
	bool ReadConfig(wchar_t* cfgname);
};

#define GAME_MANAGER() Application::GetInstance().GetGameManager()
#define NETWORK_ENGINE() Application::GetInstance().GetNetworkEngine()
#define MESSAGE_QUEUE() NETWORK_ENGINE()->GetMessageQueue()
#define TIMER_QUEUE() NETWORK_ENGINE()->GetTimerQueue()
#define DB_MANAGER() NETWORK_ENGINE()->GetDBManager()
#define USER_MANAGER() GAME_MANAGER()->GetUserManager()