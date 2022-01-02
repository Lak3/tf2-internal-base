#include "Entry/Entry.h"

DWORD APIENTRY MainThread(LPVOID lpParam)
{
	g_ModuleEntry.Load();

	while (!g_ModuleEntry.ShouldUnload())
		std::this_thread::sleep_for(std::chrono::milliseconds(420));

	g_ModuleEntry.Unload();

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	static bool s_bAttached = false;

	if ((fdwReason == DLL_PROCESS_ATTACH) && !s_bAttached)
	{
		const HANDLE hMain = CreateThread(0, 0, MainThread, hinstDLL, 0, 0);

		if (hMain)
		{
			s_bAttached = true;
			CloseHandle(hMain);
		}
	}

	return TRUE;
}