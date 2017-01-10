#include "Precompiled.h"
#include "StringUtil.h"


static const int MAX_BUFFER_COUNT = 10;
static const int BUFFER_SIZE = 8192;

static __declspec(thread) char g_MbcsBuffer[MAX_BUFFER_COUNT][BUFFER_SIZE];
static __declspec(thread) int g_MbcsBufferIndex = 0;
static __declspec(thread) wchar_t g_WcsBuffer[MAX_BUFFER_COUNT][BUFFER_SIZE];
static __declspec(thread) int g_WcsBufferIndex = 0;

const char* WcsToMbcs(const wchar_t* src)
{
	int currIndex = g_MbcsBufferIndex;

	size_t ret = wcstombs(g_MbcsBuffer[currIndex], src, BUFFER_SIZE);
	g_MbcsBuffer[currIndex][ret] = L'\0';
	g_MbcsBufferIndex = (g_MbcsBufferIndex + 1) % MAX_BUFFER_COUNT;

	return g_MbcsBuffer[currIndex];
}

const wchar_t* MbcsToWcs(const char* src)
{
	int currIndex = g_WcsBufferIndex;

	size_t ret = mbstowcs(g_WcsBuffer[currIndex], src, BUFFER_SIZE);
	g_WcsBuffer[currIndex][ret] = L'\0';
	g_MbcsBufferIndex = (g_WcsBufferIndex + 1) % MAX_BUFFER_COUNT;

	return g_WcsBuffer[currIndex];
}

// trim from start
wstring& ltrim(wstring &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
wstring& rtrim(wstring &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
wstring& trim(wstring &s)
{
	return ltrim(rtrim(s));
}