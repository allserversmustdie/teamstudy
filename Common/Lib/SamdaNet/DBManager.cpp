#include "Precompiled.h"
#include "DBManager.h"
#include "MySQLWrapper.h"


DBManager::DBManager()
{

}

DBManager::~DBManager()
{
	Close();
}

bool DBManager::Open(int idx, const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd)
{
	if (GetDBHandle(idx) != nullptr)
	{
		return false;
	}

	MySQL* db = new MySQL;
	if (!db->Open(ip, port, dbname, id, passwd))
	{
		return false;
	}

	dbHandle.insert(make_pair(idx, db));

	return true;
}

void DBManager::Close()
{
	for (auto itr = dbHandle.begin(); itr != dbHandle.end(); ++itr)
	{
		MySQL* db = itr->second;

		db->Close();

		SAFE_DELETE(db);
	}
}

MySQL* DBManager::GetDBHandle(int idx)
{
	auto itr = dbHandle.find(idx);
	if (itr == dbHandle.end())
		return nullptr;

	return dbHandle[idx];
}
