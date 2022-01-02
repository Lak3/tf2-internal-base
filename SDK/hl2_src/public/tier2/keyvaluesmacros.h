//===================== Copyright (c) Valve Corporation. All Rights Reserved. ======================
//
//
//
//==================================================================================================

#pragma once

//--------------------------------------------------------------------------------------------------
// Returns true if the passed string matches the filename style glob, false otherwise
// * matches any characters, ? matches any single character, otherwise case insensitive matching
//--------------------------------------------------------------------------------------------------
bool GlobMatch(const char* pszGlob, const char* pszString);