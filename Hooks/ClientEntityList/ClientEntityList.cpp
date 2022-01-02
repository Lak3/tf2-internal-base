#include "ClientEntityList.h"

using namespace Hooks;

void __fastcall ClientEntityList::OnAddEntity::Detour(void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle)
{
	Func.Original<FN>()(ecx, edx, pEnt, handle);
}

void __fastcall ClientEntityList::OnRemoveEntity::Detour(void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle)
{
	Func.Original<FN>()(ecx, edx, pEnt, handle);
}

void ClientEntityList::Initialize()
{
	//OnAddEntity
	{
		using namespace OnAddEntity;

		const FN pfOnAddEntity = reinterpret_cast<FN>(g_Offsets.m_dwOnAddEntity);
		XASSERT(pfOnAddEntity == nullptr);

		if (pfOnAddEntity)
			XASSERT(Func.Initialize(pfOnAddEntity, &Detour) == FAILED_TO_INITIALIZE);
	}

	//OnRemoveEntity
	{
		using namespace OnRemoveEntity;

		const FN pfOnRemoveEntity = reinterpret_cast<FN>(g_Offsets.m_dwOnRemoveEntity);
		XASSERT(pfOnRemoveEntity == nullptr);

		if (pfOnRemoveEntity)
			XASSERT(Func.Initialize(pfOnRemoveEntity, &Detour) == FAILED_TO_INITIALIZE);
	}
}