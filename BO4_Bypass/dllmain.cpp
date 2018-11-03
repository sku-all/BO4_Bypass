// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Bypass.hpp"


VOID OnAttach(HMODULE hDll) {
	bool Error = true;
	char szError[1024] = { 0 };
	Error = Bypass::GetInstance()->Init(hDll);
	if (Error != true)
	{
		sprintf_s(szError, 1024, "[Bypass] Failed to initialize Bypass (errorcode : %i)\n", GetLastError());
		OutputDebugStringA(szError);
		MessageBoxA(0, szError, "ERROR", MB_ICONERROR);
	}
	else {
		OutputDebugStringA("[Bypass] Initialized successfully.\n");
	}
	ModuleHidden::OnAttach(hDll);
}

VOID OnDetach() {
	bool Error = true;
	char szError[1024] = { 0 };
	Error = Bypass::GetInstance()->Uninit();
	if (Error != true)
	{
		sprintf_s(szError, 1024, "Failed to uninitialize Bypass (errorcode : %i)\n", GetLastError());
		MessageBoxA(0, szError, "ERROR", MB_ICONERROR);
	}
	ModuleHidden::OnDetach();
}



BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugStringA("[Bypass] DLL_PROCESS_ATTACH\n");
		OnAttach(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		OutputDebugStringA("[Bypass] DLL_PROCESS_DETACH\n");
		MessageBoxA(NULL, "BYPASS DLL_PROCESS_DETACH", NULL, MB_OK);
		OnDetach();
		break;
	}
	return TRUE;
}

