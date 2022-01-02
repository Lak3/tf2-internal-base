#include "SequenceTransitioner.h"

#include "../../Features/Vars.h"

using namespace Hooks;

void __fastcall SequenceTransitioner::CheckForSequenceChange::Detour(void* ecx, void* edx, CStudioHdr* hdr, int nCurSequence, bool bForceNewSequence, bool bInterpolate)
{
	Func.Original<FN>()(ecx, edx, hdr, nCurSequence, bForceNewSequence, bInterpolate);
}

void SequenceTransitioner::Initialize()
{
	//CheckForSequenceChange
	{
		using namespace CheckForSequenceChange;

		const auto pfCheckForSeqChange = reinterpret_cast<FN>(g_Offsets.m_dwCheckForSequenceChange);
		XASSERT(pfCheckForSeqChange == nullptr);

		if (pfCheckForSeqChange)
			XASSERT(Func.Initialize(pfCheckForSeqChange, &Detour) == FAILED_TO_INITIALIZE);
	}
}