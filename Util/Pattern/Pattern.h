#pragma once

#include <Windows.h>
#include <TlHelp32.h>

class CPattern
{
public:
	DWORD Find(const wchar_t* const szModule, const wchar_t* const szPattern);
	DWORD Find(const char* const szModule, const char* const szPattern);

private:
	DWORD FindPattern(const DWORD dwAddress, const DWORD dwLen, const wchar_t* const szPattern);
	DWORD FindPattern(const DWORD dwAddress, const DWORD dwLen, const char* const szPattern);
};

inline CPattern g_Pattern;