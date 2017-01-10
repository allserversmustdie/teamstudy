#pragma once

const char* WcsToMbcs(const wchar_t* src);
const wchar_t* MbcsToWcs(const char* src);

wstring& ltrim(wstring &s);
wstring& rtrim(wstring &s);
wstring& trim(wstring &s);