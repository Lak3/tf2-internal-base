#pragma once

#include "NetChannel.h"

using namespace Hooks;

bool __fastcall NetChannel::SendNetMsg::Detour(CNetChan* pThis, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	return Func.Original<FN>()(pThis, edx, msg, bForceReliable, bVoice);
}

int __fastcall NetChannel::SendDataGram::Detour(INetChannel* pThis, void* edx, bf_write* data)
{
	return Func.Original<FN>()(pThis, edx, data);
}

void __fastcall NetChannel::Shutdown::Detour(INetChannel* pThis, void* edx, const char* pReason)
{
	Func.Original<FN>()(pThis, edx, "Lets copy ID tech 3 engine and rename it to Source engine!!!");
}

void NetChannel::Initialize()
{
	//SendNetMsg
	{
		using namespace SendNetMsg;

		const FN pfSendNetMsg = reinterpret_cast<FN>(g_Offsets.m_dwSendNetMsg);
		XASSERT(pfSendNetMsg == nullptr);

		if (pfSendNetMsg)
			XASSERT(Func.Initialize(pfSendNetMsg, &Detour) == FAILED_TO_INITIALIZE);
	}

	//SendDataGram
	{
		using namespace SendDataGram;

		const FN pfSendDataGram = reinterpret_cast<FN>(g_Offsets.m_dwSendDataGram);
		XASSERT(pfSendDataGram == nullptr);

		if (pfSendDataGram)
			XASSERT(Func.Initialize(pfSendDataGram, &Detour) == FAILED_TO_INITIALIZE);
	}

	//Shutdown
	{
		using namespace Shutdown;

		const FN pfShutdown = reinterpret_cast<FN>(g_Offsets.m_dwShutdown);
		XASSERT(pfShutdown == nullptr);

		if (pfShutdown)
			XASSERT(Func.Initialize(pfShutdown, &Detour) == FAILED_TO_INITIALIZE);
	}
}