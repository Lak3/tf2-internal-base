#include "NetVarManager.h"

#include "../hl2_src/public/cdll_int.h"
#include "../hl2_src/public/dt_recv.h"
#include "../hl2_src/public/client_class.h"
#include "../hl2_src/public/tier1/strtools.h"

int GetOffset(RecvTable* pTable, const char* const szVar)
{
	int n; const int nMax = pTable->m_nProps;
	for (n = 0; n < nMax; n++)
	{
		const auto Prop = pTable->m_pProps[n];

		if (V_strcmp(szVar, Prop.m_pVarName) == 0)
			return Prop.GetOffset();

		if (const auto pDataTable = Prop.GetDataTable())
		{
			if (const auto nOffset = GetOffset(pDataTable, szVar))
				return nOffset + Prop.GetOffset();
		}
	}

	return 0;
}

int GetNetVar(const char* const szClass, const char* const szVar)
{
	ClientClass* pCC = I::BaseClient->GetAllClasses();

	while (pCC)
	{
		if (V_strcmp(szClass, pCC->GetName()) == 0)
			return GetOffset(pCC->m_pRecvTable, szVar);

		pCC = pCC->m_pNext;
	}

	return 0;
}