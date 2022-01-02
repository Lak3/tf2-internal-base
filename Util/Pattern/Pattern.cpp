#include "Pattern.h"

#include <Psapi.h>

#define INRANGE(x,a,b)	(x >= a && x <= b)
#define GetBits(x)		(INRANGE((x & (~0x20)),'A','F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetBytes(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

DWORD CPattern::Find(const wchar_t* const szModule, const wchar_t* const szPattern)
{
    if (const auto dwMod = reinterpret_cast<DWORD>(GetModuleHandleW(szModule)))
    {
        if (const auto& NTH = reinterpret_cast<PIMAGE_NT_HEADERS32>(dwMod + reinterpret_cast<PIMAGE_DOS_HEADER>(dwMod)->e_lfanew))
            return FindPattern(dwMod + NTH->OptionalHeader.BaseOfCode, dwMod + NTH->OptionalHeader.SizeOfCode, szPattern);
    }

    return 0x0;
}

DWORD CPattern::Find(const char* const szModule, const char* const szPattern)
{
    if (const auto dwMod = reinterpret_cast<DWORD>(GetModuleHandleA(szModule)))
    {
        if (const auto& NTH = reinterpret_cast<PIMAGE_NT_HEADERS32>(dwMod + reinterpret_cast<PIMAGE_DOS_HEADER>(dwMod)->e_lfanew))
            return FindPattern(dwMod + NTH->OptionalHeader.BaseOfCode, dwMod + NTH->OptionalHeader.SizeOfCode, szPattern);
    }

    return 0x0;
}

DWORD CPattern::FindPattern(const DWORD dwAddress, const DWORD dwLen, const wchar_t* const szPattern)
{
    auto  szPatt = szPattern;
    DWORD dwFirstMatch = 0x0;

    DWORD dwCur;
    for (dwCur = dwAddress; dwCur < dwLen; dwCur++)
    {
        if (!szPatt)
            return dwFirstMatch;

        const auto& pCurByte = *(BYTE*)dwCur;
        const auto& pBytePatt = *(BYTE*)szPatt;

        if (pBytePatt == '\?' || pCurByte == GetBytes(szPatt))
        {
            if (!dwFirstMatch)
                dwFirstMatch = dwCur;

            if (!szPatt[2])
                return dwFirstMatch;

            szPatt += (pBytePatt == '\?\?' || pBytePatt != '\?') ? 3 : 2;
        }
        else
        {
            szPatt = szPattern;
            dwFirstMatch = 0x0;
        }
    }

    return 0x0;
}

DWORD CPattern::FindPattern(const DWORD dwAddress, const DWORD dwLen, const char* const szPattern)
{
    auto  szPatt = szPattern;
    DWORD dwFirstMatch = 0x0;

    DWORD dwCur;
    for (dwCur = dwAddress; dwCur < dwLen; dwCur++)
    {
        if (!szPatt)
            return dwFirstMatch;

        const auto& pCurByte = *(BYTE*)dwCur;
        const auto& pBytePatt = *(BYTE*)szPatt;

        if (pBytePatt == '\?' || pCurByte == GetBytes(szPatt))
        {
            if (!dwFirstMatch)
                dwFirstMatch = dwCur;

            if (!szPatt[2])
                return dwFirstMatch;

            szPatt += (pBytePatt == '\?\?' || pBytePatt != '\?') ? 3 : 2;
        }
        else
        {
            szPatt = szPattern;
            dwFirstMatch = 0x0;
        }
    }

    return 0x0;
}