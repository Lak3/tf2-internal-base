//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//
#if defined( _WIN32 ) && !defined( _X360 )
#include <Windows.h>
#endif

#if !defined( DONT_PROTECT_FILEIO_FUNCTIONS )
#define DONT_PROTECT_FILEIO_FUNCTIONS // for protected_things.h
#endif

#if defined( PROTECTED_THINGS_ENABLE )
#undef PROTECTED_THINGS_ENABLE // from protected_things.h
#endif

#include "../public/tier0/dbg.h"
#include "../public/tier0/basetypes.h"
#include "../public/tier0/threadtools.h"
#include "../public/tier1/interface.h"
#include "../public/tier1/strtools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h> // getcwd
#elif POSIX
#include <dlfcn.h>
#include <unistd.h>
#define _getcwd getcwd
#endif
#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// ------------------------------------------------------------------------------------ //
// InterfaceReg.
// ------------------------------------------------------------------------------------ //
InterfaceReg* InterfaceReg::s_pInterfaceRegs = NULL;

InterfaceReg::InterfaceReg(InstantiateInterfaceFn fn, const char* pName) :
	m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

// ------------------------------------------------------------------------------------ //
// CreateInterface.
// This is the primary exported function by a dll, referenced by name via dynamic binding
// that exposes an opqaue function pointer to the interface.
//
// We have the Internal variant so Sys_GetFactoryThis() returns the correct internal 
// symbol under GCC/Linux/Mac as CreateInterface is DLL_EXPORT so its global so the loaders
// on those OS's pick exactly 1 of the CreateInterface symbols to be the one that is process wide and 
// all Sys_GetFactoryThis() calls find that one, which doesn't work. Using the internal walkthrough here
// makes sure Sys_GetFactoryThis() has the dll specific symbol and GetProcAddress() returns the module specific
// function for CreateInterface again getting the dll specific symbol we need.
// ------------------------------------------------------------------------------------ //
void* CreateInterfaceInternal(const char* pName, int* pReturnCode)
{
	InterfaceReg* pCur;

	for (pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext)
	{
		if (V_strcmp(pCur->m_pName, pName) == 0)
		{
			if (pReturnCode)
			{
				*pReturnCode = IFACE_OK;
			}
			return pCur->m_CreateFn();
		}
	}

	if (pReturnCode)
	{
		*pReturnCode = IFACE_FAILED;
	}

	return NULL;
}

void* CreateInterfaceInternal(const char* szModule, const char* szName, int* pReturnCode)
{
	if (const auto pFN = Sys_GetFactory(szModule))
		return pFN(szName, pReturnCode);

	if (pReturnCode)
		*pReturnCode = IFACE_FAILED;

	return nullptr;
}

void* CreateInterface(const char* pName, int* pReturnCode)
{
	return CreateInterfaceInternal(pName, pReturnCode);
}

void* CreateInterface(const char* szModule, const char* szName, int* pReturnCode)
{
	return CreateInterfaceInternal(szModule, szName, pReturnCode);
}

#ifdef POSIX
// Linux doesn't have this function so this emulates its functionality
void* GetModuleHandle(const char* name)
{
	void* handle;

	if (name == NULL)
	{
		// hmm, how can this be handled under linux....
		// is it even needed?
		return NULL;
	}

	if ((handle = dlopen(name, RTLD_NOW)) == NULL)
	{
		printf("DLOPEN Error:%s\n", dlerror());
		// couldn't open this file
		return NULL;
	}

	// read "man dlopen" for details
	// in short dlopen() inc a ref count
	// so dec the ref count by performing the close
	dlclose(handle);
	return handle;
}
#endif

#if defined( _WIN32 ) && !defined( _X360 )
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a function, given a module
// Input  : pModuleName - module name
//			*pName - proc name
//-----------------------------------------------------------------------------
static void* Sys_GetProcAddress(const char* pModuleName, const char* pName)
{
	HMODULE hModule = (HMODULE)GetModuleHandleA(pModuleName);
#ifdef WIN32
	return GetProcAddress(hModule, pName);
#else
	return (void*)dlsym((void*)hModule, pName);
#endif
}

#if !defined(LINUX)
static void* Sys_GetProcAddress(HMODULE hModule, const char* pName)
{
#ifdef WIN32
	return GetProcAddress(hModule, pName);
#else
	return (void*)dlsym((void*)hModule, pName);
#endif
}
#endif

bool Sys_IsDebuggerPresent()
{
	return Plat_IsInDebugSession();
}

struct ThreadedLoadLibaryContext_t
{
	const char* m_pLibraryName;
	HMODULE m_hLibrary;
};

#ifdef _WIN32

// wraps LoadLibraryEx() since 360 doesn't support that
static HMODULE InternalLoadLibrary(const char* pName, Sys_Flags flags)
{
#if defined(_X360)
	return LoadLibrary(pName);
#else
	if (flags & SYS_NOLOAD)
		return GetModuleHandleA(pName);
	else
		return LoadLibraryExA(pName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
#endif
}
unsigned ThreadedLoadLibraryFunc(void* pParam)
{
	ThreadedLoadLibaryContext_t* pContext = (ThreadedLoadLibaryContext_t*)pParam;
	pContext->m_hLibrary = InternalLoadLibrary(pContext->m_pLibraryName, SYS_NOFLAGS);
	return 0;
}

#endif // _WIN32

static bool s_bRunningWithDebugModules = false;

//-----------------------------------------------------------------------------
// Purpose: Determine if any debug modules were loaded
//-----------------------------------------------------------------------------
bool Sys_RunningWithDebugModules()
{
	if (!s_bRunningWithDebugModules)
	{
#if 0 //def IS_WINDOWS_PC
		char chMemoryName[MAX_PATH];
		DebugKernelMemoryObjectName(chMemoryName);

		HANDLE hObject = OpenFileMapping(FILE_MAP_READ, FALSE, chMemoryName);
		if (hObject && hObject != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hObject);
			s_bRunningWithDebugModules = true;
		}
#endif
	}
	return s_bRunningWithDebugModules;
}


//-----------------------------------------------------------------------------
// Purpose: Unloads a DLL/component from
// Input  : *pModuleName - filename of the component
// Output : opaque handle to the module (hides system dependency)
//-----------------------------------------------------------------------------
void Sys_UnloadModule(HMODULE pModule)
{
	if (!pModule)
		return;

	CloseHandle(pModule);
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a function, given a module
// Input  : module - windows HMODULE from Sys_LoadModule() 
//			*pName - proc name
// Output : factory for this module
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactory(HMODULE pModule)
{
	if (!pModule)
		return NULL;

#ifdef _WIN32
	return reinterpret_cast<CreateInterfaceFn>(Sys_GetProcAddress(pModule, CREATEINTERFACE_PROCNAME));
#elif defined(POSIX)
	// Linux gives this error:
	//../public/interface.cpp: In function `IBaseInterface *(*Sys_GetFactory
	//(CSysModule *)) (const char *, int *)':
	//../public/interface.cpp:154: ISO C++ forbids casting between
	//pointer-to-function and pointer-to-object
	//
	// so lets get around it :)
	return (CreateInterfaceFn)(GetProcAddress((void*)hDLL, CREATEINTERFACE_PROCNAME));
#endif
}

//-----------------------------------------------------------------------------
// Purpose: returns the instance of this module
// Output : interface_instance_t
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactoryThis(void)
{
	return &CreateInterfaceInternal;
}

//-----------------------------------------------------------------------------
// Purpose: returns the instance of the named module
// Input  : *pModuleName - name of the module
// Output : interface_instance_t - instance of that module
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactory(const char* pModuleName)
{
#ifdef _WIN32
	return static_cast<CreateInterfaceFn>(Sys_GetProcAddress(pModuleName, CREATEINTERFACE_PROCNAME));
#elif defined(POSIX)
	// see Sys_GetFactory( CSysModule *pModule ) for an explanation
	return (CreateInterfaceFn)(Sys_GetProcAddress(pModuleName, CREATEINTERFACE_PROCNAME));
#endif
}

//-----------------------------------------------------------------------------
// Purpose: get the interface for the specified module and version
// Input  : 
// Output : 
//-----------------------------------------------------------------------------
bool Sys_LoadInterface(
	const char* pModuleName,
	const char* pInterfaceVersionName,
	CSysModule** pOutModule,
	void** pOutInterface)
{
	HMODULE pMod = GetModuleHandleA(pModuleName);

	if (!pMod)
		return false;

	CreateInterfaceFn fn = Sys_GetFactory(pMod);
	if (!fn)
	{
		Sys_UnloadModule(pMod);
		return false;
	}

	*pOutInterface = fn(pInterfaceVersionName, NULL);
	if (!(*pOutInterface))
	{
		Sys_UnloadModule(pMod);
		return false;
	}

	if (pOutModule)
		*pOutModule = reinterpret_cast<CSysModule*>(pMod);

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Place this as a singleton at module scope (e.g.) and use it to get the factory from the specified module name.  
// 
// When the singleton goes out of scope (.dll unload if at module scope),
//  then it'll call Sys_UnloadModule on the module so that the refcount is decremented 
//  and the .dll actually can unload from memory.
//-----------------------------------------------------------------------------
CDllDemandLoader::CDllDemandLoader(char const* pchModuleName) :
	m_pchModuleName(pchModuleName),
	m_hModule(0),
	m_bLoadAttempted(false)
{
}

CDllDemandLoader::~CDllDemandLoader()
{
	Unload();
}

CreateInterfaceFn CDllDemandLoader::GetFactory()
{
	if (!m_hModule && !m_bLoadAttempted)
	{
		m_bLoadAttempted = true;
		m_hModule = GetModuleHandleA(m_pchModuleName);
	}

	if (!m_hModule)
	{
		return NULL;
	}

	return Sys_GetFactory(m_hModule);
}

void CDllDemandLoader::Unload()
{
	if (m_hModule)
	{
		Sys_UnloadModule(m_hModule);
		m_hModule = 0;
	}
}