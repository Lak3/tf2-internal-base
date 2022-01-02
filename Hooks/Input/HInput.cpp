#include "HInput.h"

using namespace Hooks;

bool __fastcall Input::WriteUsercmdDeltaToBuffer::Detour(void* ecx, void* edx, bf_write* buf, int from, int to, bool isnewcommand)
{
	return Table.Original<FN>(Index)(ecx, edx, buf, from, to, isnewcommand);
}

void __fastcall Input::EncodeUserCmdToBuffer::Detour(void* ecx, void* edx, bf_write& buf, int slot)
{
	Table.Original<FN>(Index)(ecx, edx, buf, slot);
}

void __fastcall Input::DecodeUserCmdFromBuffer::Detour(void* ecx, void* edx, bf_read& buf, int slot)
{
	Table.Original<FN>(Index)(ecx, edx, buf, slot);
}

CUserCmd* __fastcall Input::GetUserCmd::Detour(void* ecx, void* edx, int sequence_number)
{
	return Table.Original<FN>(Index)(ecx, edx, sequence_number);
}

void Input::Initialize()
{
	XASSERT(Table.Initialize(I::Input) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&WriteUsercmdDeltaToBuffer::Detour, WriteUsercmdDeltaToBuffer::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&EncodeUserCmdToBuffer::Detour, EncodeUserCmdToBuffer::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&DecodeUserCmdFromBuffer::Detour, DecodeUserCmdFromBuffer::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&GetUserCmd::Detour, GetUserCmd::Index) == FAILED_TO_HOOK);
}