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

	return 0;
}

