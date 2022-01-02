//===================== Copyright (c) Valve Corporation. All Rights Reserved. ======================
//
//
//
//==================================================================================================

#include "../public/tier1/strtools.h"

//--------------------------------------------------------------------------------------------------
// Returns true if the passed string matches the filename style glob, false otherwise
// * matches any characters, ? matches any single character, otherwise case insensitive matching
//--------------------------------------------------------------------------------------------------
bool GlobMatch(const char* pszGlob, const char* pszString)
{
	while ((*pszString != '\0') && (*pszGlob != '*'))
	{
		if ((V_strnicmp(pszGlob, pszString, 1) != 0) && (*pszGlob != '?'))
		{
			return false;
		}

		++pszGlob;
		++pszString;
	}

	const char* pszGlobTmp = nullptr;
	const char* pszStringTmp = nullptr;

	while (*pszString)
	{
		if (*pszGlob == '*')
		{
			++pszGlob;

			if (*pszGlob == '\0')
			{
				return true;
			}

			pszGlobTmp = pszGlob;
			pszStringTmp = pszString + 1;
		}
		else if ((V_strnicmp(pszGlob, pszString, 1) == 0) || (*pszGlob == '?'))
		{
			++pszGlob;
			++pszString;
		}
		else
		{
			pszGlob = pszGlobTmp;
			pszString = pszStringTmp++;
		}
	}

	while (*pszGlob == '*')
	{
		++pszGlob;
	}

	return *pszGlob == '\0';
}