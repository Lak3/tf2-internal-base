#pragma once

class CGlobals
{
public:
	int m_nLocalIndex = 0;
	int m_nMaxClients = 0;
	int m_nMaxEntities = 0;
	int m_nScreenHeight = 0;
	int m_nScreenWidht = 0;

	bool m_bIsInGame = false;
	bool m_bIsGameUIVisible = false;
};

inline CGlobals g_Globals;