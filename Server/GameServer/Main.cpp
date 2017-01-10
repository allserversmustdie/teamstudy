#include "Precompiled.h"
#include "SamdaNet.h"
#include "ClientDispatcher.h"
#include "Log.h"
#include <mysql.h>
#include "MySQLWrapper.h"
#include "StringUtil.h"
#include "DBDispatcher.h"
#include "Application.h"

int wmain(int argc, wchar_t* argv[])
{
	Application& app = Application::GetInstance();

	app.Init(argc, argv);
	app.Process();
	app.Close();
/*
	setlocale(LC_ALL, "ko_KR.utf8");
	
	wchar_t* server_ip = L"127.0.0.1";
	unsigned short server_port = 9999;

	ClientDispatcher disp;
	GameDBDispatcher db_disp;
*/
/*
	MySQL db;
	db.Open(L"52.79.45.119", 3306, L"test", L"goobobsa", L"rneogns");
	db.ExecuteQuery(L"select postal_id, unix_timestamp(occ_time), gold, hash from postal");

	unsigned int postal_id = 0;
	int unix_timestamp = 0;
	int gold = 0;
	char hash[33] = { 0, };

	int rows = (int)db.GetNumOfRows();
	for (int i = 0; i < rows; ++i)
	{
		db.Fetch();

		db.GetInt(postal_id);
		db.GetInt(unix_timestamp);
		db.GetInt(gold);

		memset(hash, 0, sizeof(hash));
		db.GetBinary(hash, 32);
		wprintf(L"id: %u, time: %u, gold: %d, hash: %s\n", postal_id, unix_timestamp, gold, MbcsToWcs(hash));
	}
*/
/*
	SamdaNet netservice;
	if (netservice.Open(server_ip, server_port, &disp) == false)
		return 1;

	LOG_INFO(L"Server started! ip: %s, port: %d", server_ip, server_port);

	getchar();

	netservice.Stop();
*/
	return 0;
}

