#pragma once

class KeyValues
{
public:
	int m_iKeyName;
	char* m_sValue;
	wchar_t* m_wsValue;

	union
	{
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char m_iDataType;
	char m_bHasEscapeSequences;
	char m_bEvaluateConditionals;
	char unused[1];

	KeyValues* m_pPeer;
	KeyValues* m_pSub;
	KeyValues* m_pChain;
};

class CKeyValUtils
{
public:
	bool LoadFromBuffer(KeyValues* key_value, char const* resource_name, const char* buffer, void* file_system = 0, const char* path_id = 0);
	KeyValues* Initialize(KeyValues* key_value, char* name);
};

inline CKeyValUtils g_KeyValues;