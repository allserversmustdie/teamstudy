#include "Precompiled.h"
#include "Log.h"
#include "TimeUtil.h"

#define LINE_BUFFER 32768

LogFile::LogFile(LogType type, wstring prefix, bool writeTime, bool writeFuncName, bool writePerHour)
	: fp(0)
	, logType(type)
	, prefix(prefix)
	, isWriteTime(writeTime)
	, isWriteFuncName(writeFuncName)
	, isWritePerHour(writePerHour)
{
}

LogFile::~LogFile()
{
	if (fp != NULL)
		fclose(fp);
}

void LogFile::openFile()
{
	//wchar_t path[MAX_PATH_LEN] = { 0, };
	wchar_t filename[MAX_FILENAME_LEN] = { 0, };

	struct tm nowTM;
	TimeUtil::getCurrTM(&nowTM);

	if (isWritePerHour)
		wsprintf(filename, L"%s%04d%02d%02d-%02d.log", prefix.c_str(), nowTM.tm_year + 1900, nowTM.tm_mon + 1, nowTM.tm_mday, nowTM.tm_hour);
	else
		wsprintf(filename, L"%s%04d%02d%02d.log", prefix.c_str(), nowTM.tm_year + 1900, nowTM.tm_mon + 1, nowTM.tm_mday);

	fp = _wfopen(filename, L"ab");
}

void LogFile::write(const wchar_t* funcName, int lineNo, const wchar_t* message)
{
	wchar_t header[128] = { 0, };
	wchar_t buffer[LINE_BUFFER] = { 0, };

	if (fp == NULL)
		openFile();

	if (isWriteTime)
	{
		struct tm nowTM;
		TimeUtil::getCurrTM(&nowTM);
		wsprintf(header, L"[%02d:%02d:%02d]", nowTM.tm_hour, nowTM.tm_min, nowTM.tm_sec);
	}

	if (isWriteFuncName)
		wsprintf(header, L"%s %s(%d)", header, funcName, lineNo);

	fwprintf(fp, L"%s: %s\r\n", header, message);
	wprintf(L"%s: %s\r\n", header, message);

	fflush(fp);
}

Log::Log()
{
	_logFiles.push_back(LogFile(LogType::DEBUG	, L"Debug"	, true, true, false));
	_logFiles.push_back(LogFile(LogType::CORE	, L"Core"	, true, true, false));
	_logFiles.push_back(LogFile(LogType::INFO	, L"Info"	, true, true, false));
	_logFiles.push_back(LogFile(LogType::CRI	, L"Cri"	, true, true, false));
}

Log& Log::getInstance()
{
	static Log inst;
	return inst;
}

void Log::writeToFile(LogType type, const wchar_t* funcName, int lineNo, const wchar_t* message)
{
	_logFiles[type].write(funcName, lineNo, message);
}

LogHelper::LogHelper(LogType type, const wchar_t* funcName, int lineNo)
	: logType(type)
	, funcName(funcName)
	, lineNo(lineNo)
{
}

void LogHelper::operator()(const wchar_t* message, ...)
{
	wchar_t buffer[LINE_BUFFER] = { 0, };

	va_list args;
	va_start(args, message);
	wvsprintf(buffer, message, args);
	va_end(args);

	Log::getInstance().writeToFile(logType, funcName, lineNo, buffer);
}