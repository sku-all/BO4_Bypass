// BO4_Bypass.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Bypass.hpp"

#pragma region Member
Bypass* Bypass::Instance;
HANDLE Bypass::hEvent, Bypass::hProcessHandle, Bypass::hThread;
Bypass::p_EnumWindows Bypass::o_EnumWindows;
Bypass::p_NtCreateFile Bypass::o_NtCreateFile;
Bypass::p_RtlInitUnicodeString Bypass::o_RtlInitUnicodeString;
Bypass::p_NtQueryVirtualMemory Bypass::o_NtQueryVirtualMemory;
Bypass::p_NtOpenProcess Bypass::o_NtOpenProcess;
Bypass::p_GetWindowThreadProcessId Bypass::o_GetWindowThreadProcessId;
Bypass::p_NtQuerySystemInformation Bypass::o_NtQuerySystemInformation;
Bypass::p_NtQueryInformationThread Bypass::o_NtQueryInformationThread;
Bypass::p_NtReadVirtualMemory Bypass::o_NtReadVirtualMemory;
Bypass::p_ZwQueryInformationProcess Bypass::o_ZwQueryInformationProcess;
Bypass::p_ZwQueryInformationProcess Bypass::o_NtWow64QueryInformationProcess64;
Bypass::p_NtWow64QueryVirtualMemory64 Bypass::o_NtWow64QueryVirtualMemory64;
Bypass::p_NtGetContextThread Bypass::o_NtGetContextThread;
Bypass::p_NtOpenThread Bypass::o_NtOpenThread;
Bypass::p_NtWow64ReadVirtualMemory64 Bypass::o_NtWow64ReadVirtualMemory64;
Bypass::p_NtReadFile Bypass::o_NtReadFile;
DWORD_PTR Bypass::hModule;
#pragma endregion Member
Bypass* Bypass::GetInstance()
{
	if (!Instance)
		Instance = new Bypass;
	return Instance;
}

Bypass::Bypass()
{
	Instance = 0;
	hEvent = 0;
	hModule = 0;
	hProcessHandle = 0;
	hThread = 0;
	o_NtCreateFile = 0;
	o_RtlInitUnicodeString = 0;
	o_NtQueryVirtualMemory = 0;
	o_NtOpenProcess = 0;
	o_GetWindowThreadProcessId = 0;
	o_NtQuerySystemInformation = 0;
	o_NtQueryInformationThread = 0;
	o_NtReadVirtualMemory = 0;
	o_ZwQueryInformationProcess = 0;
	o_NtWow64QueryInformationProcess64 = 0;
	o_NtWow64QueryVirtualMemory64 = 0;
	o_NtGetContextThread = 0;
	o_NtOpenThread = 0;
	o_NtWow64ReadVirtualMemory64 = 0;
	o_NtReadFile = 0;
}
Bypass::~Bypass()
{
	if (Instance)
		delete Instance;
}

bool Bypass::Init(HMODULE hDll)
{
	VirtualizerStart();
#if (LOG_STATE == 1)
	DbgLog::Log("============================== Call of Duty Black Ops 4 BYPASS ===============================");
	DbgLog::Log("| LOADING SETTINGS |");
	DbgLog::Log("DISABLE_DLLDETECTION = %d", DISABLE_DLLDETECTION);
	DbgLog::Log("DISABLE_THREADDETECTION = %d", DISABLE_THREADDETECTION);
	DbgLog::Log("DISABLE_PROCESSDETECTION = %d", DISABLE_PROCESSDETECTION);
	DbgLog::Log("DISABLE_READFILE = %d", DISABLE_READFILE);
	DbgLog::Log("LOG_STATE = %d", LOG_STATE);
	DbgLog::Log("LOG_CREATESEMPAPHORE = %d", LOG_CREATESEMPAPHORE);
	DbgLog::Log("LOG_NTQUERYVIRTUALMEMORY = %d", LOG_NTQUERYVIRTUALMEMORY);
	DbgLog::Log("LOG_NTWOW64QUERYVIRTUALMEMORY64 = %d", LOG_NTWOW64QUERYVIRTUALMEMORY64);
	DbgLog::Log("LOG_NTOPENPROCESS = %d", LOG_NTOPENPROCESS);
	DbgLog::Log("LOG_GETWINDOTHREADPROCESSID = %d", LOG_GETWINDOTHREADPROCESSID);
	DbgLog::Log("LOG_NTQUERYSYSTEMINFORMATION = %d", LOG_NTQUERYSYSTEMINFORMATION);
	DbgLog::Log("LOG_NTGETCONTEXTTHREAD = %d", LOG_NTGETCONTEXTTHREAD);
	DbgLog::Log("LOG_NTOPENTHREAD = %d", LOG_NTOPENTHREAD);
	DbgLog::Log("USE_VMPROTECT = %d", USE_VMPROTECT);
	DbgLog::Log("| SETTINGS LOADED |");
	DbgLog::Log("| STARTING BYPASS |");
	DbgLog::Log("Bypass::Init START 0x%X", hDll);
#endif
	hEvent = CreateEventA(0, 0, 0, 0);
	hProcessHandle = (OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId()));
	o_NtQueryInformationThread = o_NtQueryInformationThread ?
		o_NtQueryInformationThread : reinterpret_cast<p_NtQueryInformationThread>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread"));
	o_NtWow64ReadVirtualMemory64 = o_NtWow64ReadVirtualMemory64 ? o_NtWow64ReadVirtualMemory64 : reinterpret_cast<p_NtWow64ReadVirtualMemory64>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtWow64ReadVirtualMemory64"));
	hModule = reinterpret_cast<DWORD_PTR>(hDll);
	if (!hProcessHandle || hProcessHandle == INVALID_HANDLE_VALUE ||
		!o_NtQueryInformationThread ||
		!hEvent ||
		!detour_Functions(true))
	{
#if (LOG_STATE == 1)
		DbgLog::Log("Bypass::Init FAIL 0x0 = FALSE");
		DbgLog::Log("| END BYPASS |");
#endif
		return false;
	}

#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::Init END");
	DbgLog::Log("| END BYPASS |");
	DbgLog::Log("============================== Call of Duty Black Ops 4 BYPASS ===============================");
#endif
	VirtualizerEnd();
	return true;
}

bool Bypass::Uninit()
{
	VirtualizerStart();
#if (LOG_STATE == 1)
	DbgLog::Log("============================== Call of Duty Black Ops 4 BYPASS ===============================");
	DbgLog::Log("Bypass::Uninit START");
#endif
	CloseHandle(hProcessHandle);
	if (hEvent)
	{
		SetEvent(hEvent);
		CloseHandle(hEvent);
	}
	if (hThread)
	{
		TerminateThread(hThread, 0);
		CloseHandle(hThread);
	}

	if (!detour_Functions(false))
	{
#if (LOG_STATE == 1)
		DbgLog::Log("Bypass::Uninit FAIL 0x0 = FALSE");
		return false;
#endif
	}
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::Uninit END");
	DbgLog::Log("============================== Call of Duty Black Ops 4 BYPASS ===============================");
#endif
	VirtualizerEnd();
	CloseHandle(hProcessHandle);
	return true;
}

#pragma region Hooks

bool Bypass::detour_Functions(bool Status)
{
	INT bStatus = 0;
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::detour_Functions START 0x%X", Status);
#endif

#if (DISABLE_THREADDETECTION == 1)
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_THREADDETECTION START 0x%X", Status);
#endif
	VirtualizerStart();
	o_NtGetContextThread = o_NtGetContextThread ? o_NtGetContextThread : reinterpret_cast<p_NtGetContextThread>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtGetContextThread"));
	if (!o_NtGetContextThread)
		bStatus = 4;
	o_NtOpenThread = o_NtOpenThread ? o_NtOpenThread : reinterpret_cast<p_NtOpenThread>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtOpenThread"));
	if (!o_NtOpenThread)
		bStatus = 5;
	if (!o_NtGetContextThread ||
		!o_NtOpenThread ||
		DetourTransactionBegin() != NO_ERROR ||
		DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtGetContextThread, NtGetContextThread_Hook) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtOpenThread, NtOpenThread_Hook) != NO_ERROR ||
		DetourTransactionCommit() != NO_ERROR)
		if (!bStatus)
			bStatus = 6;
	VirtualizerEnd();
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_THREADDETECTION END 0x%X", bStatus);
#endif
#endif

#if (DISABLE_DLLDETECTION == 1)
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_DLLDETECTION START 0x%X", Status);
#endif
	VirtualizerStart();
	o_NtQueryVirtualMemory = o_NtQueryVirtualMemory ? o_NtQueryVirtualMemory : reinterpret_cast<p_NtQueryVirtualMemory>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryVirtualMemory"));
	if (!o_NtQueryVirtualMemory)
		bStatus = 10;
	o_NtWow64QueryVirtualMemory64 = o_NtWow64QueryVirtualMemory64 ? o_NtWow64QueryVirtualMemory64 : reinterpret_cast<p_NtWow64QueryVirtualMemory64>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtWow64QueryVirtualMemory64"));
	if (o_NtWow64QueryVirtualMemory64)
	{
		if (DetourTransactionBegin() != NO_ERROR ||
			DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
			(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtWow64QueryVirtualMemory64, NtWow64QueryVirtualMemory64_Hook) != NO_ERROR ||
			DetourTransactionCommit() != NO_ERROR)
			if (!bStatus)
				bStatus = 11;
	}
	if (!o_NtQueryVirtualMemory ||
		DetourTransactionBegin() != NO_ERROR ||
		DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtQueryVirtualMemory, NtQueryVirtualMemory_Hook) != NO_ERROR ||
		DetourTransactionCommit() != NO_ERROR)
		if (!bStatus)
			bStatus = 12;
	VirtualizerEnd();
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_DLLDETECTION END 0x%X", bStatus);
#endif
#endif

#if (DISABLE_PROCESSDETECTION == 1)
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_PROCESSDETECTION START 0x%X", Status);
#endif
	VirtualizerStart();
	o_NtOpenProcess = o_NtOpenProcess ? o_NtOpenProcess : reinterpret_cast<p_NtOpenProcess>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtOpenProcess"));
	if (!o_NtOpenProcess)
		bStatus = 13;
	o_EnumWindows = o_EnumWindows ? o_EnumWindows : reinterpret_cast<p_EnumWindows>(GetProcAddress(GetModuleHandle("user32.dll"), "EnumWindows"));
	if (!o_EnumWindows)
		bStatus = 14;
	o_GetWindowThreadProcessId = o_GetWindowThreadProcessId ? o_GetWindowThreadProcessId : reinterpret_cast<p_GetWindowThreadProcessId>(GetProcAddress(GetModuleHandle("user32.dll"), "GetWindowThreadProcessId"));
	if (!o_GetWindowThreadProcessId)
		bStatus = 15;
	o_NtQuerySystemInformation = o_NtQuerySystemInformation ? o_NtQuerySystemInformation : reinterpret_cast<p_NtQuerySystemInformation>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQuerySystemInformation"));
	if (!o_NtQuerySystemInformation)
		bStatus = 16;
	o_NtReadVirtualMemory = o_NtReadVirtualMemory ? o_NtReadVirtualMemory : reinterpret_cast<p_NtReadVirtualMemory>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtReadVirtualMemory"));
	if (!o_NtReadVirtualMemory)
		bStatus = 17;
	if (!o_NtOpenProcess ||
		!o_NtQuerySystemInformation ||
		!o_NtReadVirtualMemory ||
		!o_GetWindowThreadProcessId ||
		DetourTransactionBegin() != NO_ERROR ||
		DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtQuerySystemInformation, NtQuerySystemInformation_Hook) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_EnumWindows, EnumWindows_Hook) != NO_ERROR ||
		//(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_GetWindowThreadProcessId, GetWindowThreadProcessId_Hook) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtOpenProcess, NtOpenProcess_Hook) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtReadVirtualMemory, NtReadVirtualMemory_Hook) != NO_ERROR ||
		DetourTransactionCommit() != NO_ERROR)
		if (!bStatus)
			bStatus = 20;
	VirtualizerEnd();
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_PROCESSDETECTION END 0x%X", bStatus);
#endif
#endif

#if (DISABLE_READFILE == 1)
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_READFILE START 0x%X", Status);
#endif
	o_NtCreateFile = o_NtCreateFile ? o_NtCreateFile : reinterpret_cast<p_NtCreateFile>(reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandle("ntdll.dll"), "NtCreateFile")));
	if (!o_NtCreateFile)
		bStatus = 21;
	o_RtlInitUnicodeString = o_RtlInitUnicodeString ? o_RtlInitUnicodeString : reinterpret_cast<p_RtlInitUnicodeString>(reinterpret_cast<DWORD_PTR>(GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlInitUnicodeString")));
	if (!o_RtlInitUnicodeString)
		bStatus = 22;

	if (DetourTransactionBegin() != NO_ERROR ||
		DetourUpdateThread(GetCurrentThread()) != NO_ERROR ||
		(Status ? DetourAttach : DetourDetach)(&(PVOID&)o_NtCreateFile, NtCreateFile_Hook) != NO_ERROR ||
		DetourTransactionCommit() != NO_ERROR)
		if (!bStatus)
			bStatus = 23;
#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::DISABLE_READFILE END 0x%X", bStatus);
#endif
#endif


#if (LOG_STATE == 1)
	DbgLog::Log("Bypass::detour_Functions END 0x%X", bStatus);
#endif
	return bStatus ? false : true;
}


NTSTATUS NTAPI Bypass::NtQueryVirtualMemory_Hook(HANDLE ProcessHandle, PVOID BaseAddress, int MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength)
{
	VirtualizerStart();
	auto IsValidHeader = [](PVOID BaseAddress)->BOOL
	{
		BYTE bd[3];
		ZeroMemory(bd, 3);
		if (!o_NtReadVirtualMemory(GetCurrentProcess(), reinterpret_cast<PVOID>(BaseAddress), &bd, 3, 0) &&
			bd[0] == 0x4D &&
			bd[1] == 0x5A &&
			bd[2] == 0x90)
			return true;
		return false;
	};

	PMEMORY_BASIC_INFORMATION pbi = reinterpret_cast<PMEMORY_BASIC_INFORMATION>(MemoryInformation);

	NTSTATUS Status = o_NtQueryVirtualMemory(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
#if (LOG_NTQUERYVIRTUALMEMORY == 1)
	DbgLog::Log("NtQueryVirtualMemory - %d, 0x%016llX", GetCurrentProcessId(), BaseAddress);
#endif
	if (MemoryInformationClass == 0 && pbi)
	{
		pbi->AllocationProtect = PAGE_NOACCESS;
		pbi->RegionSize = 0;
		pbi->State = MEM_FREE;
		pbi->Protect = 0;
		pbi->Type = MEM_PRIVATE;
		return 0;
	}
	VirtualizerEnd();
	return Status;
};

NTSTATUS NTAPI Bypass::NtWow64QueryVirtualMemory64_Hook(HANDLE ProcessHandle, PVOID64 BaseAddress, int MemoryInformationClass, PVOID MemoryInformation, ULONGLONG MemoryInformationLength, PULONGLONG ReturnLength)
{
	VirtualizerStart();
	auto IsValidHeader = [](ULONGLONG BaseAddress)->BOOL
	{
		BYTE bd[3];
		ZeroMemory(bd, 3);
		if (!o_NtWow64ReadVirtualMemory64(GetCurrentProcess(), reinterpret_cast<PVOID64>(BaseAddress), &bd, 3, 0) &&
			bd[0] == 0x4D &&
			bd[1] == 0x5A &&
			bd[2] == 0x90)
			return true;
		return false;
	};
	PMEMORY_BASIC_INFORMATION64 pbi = reinterpret_cast<PMEMORY_BASIC_INFORMATION64>(MemoryInformation);

	NTSTATUS Status = 0;
#if (LOG_NTWOW64QUERYVIRTUALMEMORY64 == 1)
	DbgLog::Log("LOG_NTWOW64QUERYVIRTUALMEMORY64 - %d, 0x%X", GetCurrentProcessId(), BaseAddress);
#endif
	if (MemoryInformationClass == 0 && pbi)
	{
		pbi->AllocationProtect = PAGE_NOACCESS;
		pbi->RegionSize = 0;
		pbi->State = MEM_FREE;
		pbi->Protect = 0;
		pbi->Type = MEM_PRIVATE;
		return 0;
	}
	VirtualizerEnd();
	if (o_NtWow64QueryVirtualMemory64)
		return o_NtWow64QueryVirtualMemory64(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
	return 0;
};

NTSTATUS NTAPI Bypass::NtOpenProcess_Hook(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PVOID Inject_GameId)
{
	VirtualizerStart();

	typedef struct _CLIENT_ID {
		HANDLE UniqueProcess;
		HANDLE UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;

	NTSTATUS Status = 0;
	DWORD ProcessId = 0;
	PCLIENT_ID Inject_GameID = reinterpret_cast<PCLIENT_ID>(Inject_GameId);

	if (Inject_GameID->UniqueProcess != reinterpret_cast<HANDLE>((ULONG64)GetCurrentProcessId()))
	{
#if (LOG_NTOPENPROCESS == 1)
		DbgLog::Log("NtOpenProcess - %d, %d", Inject_GameID->UniqueProcess, GetCurrentProcessId());
#endif
		*ProcessHandle = GetCurrentProcess();
		return 0;
	}
	if (Inject_GameID->UniqueProcess == reinterpret_cast<HANDLE>((ULONG64)GetCurrentProcessId()))
	{
#if (LOG_NTOPENPROCESS == 1)
		DbgLog::Log("NtOpenProcess - %d, %d", Inject_GameID->UniqueProcess, GetCurrentProcessId());
#endif
	}

	VirtualizerEnd();
	Status = o_NtOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, Inject_GameId);
	return Status;
};

BOOL CALLBACK Bypass::EnumWindows_Hook_EnumWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD dwProcessId = 0;
	o_GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId != GetCurrentProcessId()) {
		// return TRUE; // 这里会校验回调数目
		hWnd = (HWND)-1;
	}
	return (*reinterpret_cast<WNDENUMPROC*>(reinterpret_cast<LPVOID*>(lParam)))
		(hWnd, *reinterpret_cast<LPARAM*>(reinterpret_cast<LPVOID*>(lParam) + 1));
}

BOOL WINAPI Bypass::EnumWindows_Hook(WNDENUMPROC lpEnumFunc, LPARAM lParam) {
	VirtualizerStart();
	BOOL ReturnStatus = false;

	LPVOID lParamTemp[2] = { 0 };
	*reinterpret_cast<WNDENUMPROC*>(lParamTemp) = lpEnumFunc;
	*reinterpret_cast<LPARAM*>(lParamTemp + 1) = lParam;

	ReturnStatus = o_EnumWindows(EnumWindows_Hook_EnumWindowsProc, (LPARAM)lParamTemp);
	VirtualizerEnd();
	return ReturnStatus;
}

DWORD WINAPI Bypass::GetWindowThreadProcessId_Hook(HWND hWnd, LPDWORD lpdwProcessId)
{
	VirtualizerStart();
	DWORD ReturnStatus = 0;
	char Window[1024];
	ZeroMemory(Window, 1024);

	ReturnStatus = o_GetWindowThreadProcessId(hWnd, lpdwProcessId);
	// 游戏会检测前台窗口的进程ID
	if (lpdwProcessId)
		*lpdwProcessId = GetCurrentProcessId();
#if (LOG_GETWINDOTHREADPROCESSID == 1)
	if (GetWindowText(hWnd, Window, 1024) && Window)
		DbgLog::Log("GetWindowThreadProcessId - %s, %d", Window, GetCurrentProcessId());
#endif
	return true;
	VirtualizerEnd();
	return ReturnStatus;
};

NTSTATUS NTAPI Bypass::NtQuerySystemInformation_Hook(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
{
	VirtualizerStart();
	typedef struct _MY_SYSTEM_PROCESS_INFORMATION
	{
		ULONG                   NextEntryOffset;
		ULONG                   NumberOfThreads;
		LARGE_INTEGER           Reserved[3];
		LARGE_INTEGER           CreateTime;
		LARGE_INTEGER           UserTime;
		LARGE_INTEGER           KernelTime;
		UNICODE_STRING          ImageName;
		ULONG                   BasePriority;
		HANDLE                  ProcessId;
		HANDLE                  InheritedFromProcessId;
	} MY_SYSTEM_PROCESS_INFORMATION, *PMY_SYSTEM_PROCESS_INFORMATION;

	typedef struct _SYSTEM_HANDLE
	{
		ULONG ProcessId;
		BYTE ObjectTypeNumber;
		BYTE Flags;
		USHORT Handle;
		PVOID Object;
		ACCESS_MASK GrantedAccess;
	} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

	typedef struct _SYSTEM_HANDLE_INFORMATION
	{
		ULONG HandleCount;
		SYSTEM_HANDLE Handles[1];
	} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

	DWORD Old = 0;
	NTSTATUS Status = -1;
	PMY_SYSTEM_PROCESS_INFORMATION ppi = 0;
	PSYSTEM_HANDLE_INFORMATION psi = 0;

	Status = o_NtQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(Status))
	{
		switch (SystemInformationClass)
		{
		case 0:
			break;
		case 1:
			break;
		case 5:
			break;
		case 16: // NT_HANDLE_LIST
			psi = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(SystemInformation);
			if (psi)
			{
				for (unsigned int i = 0; i < psi->HandleCount; i++)
				{
#if (LOG_NTQUERYSYSTEMINFORMATION == 1)
					DbgLog::Log("NtQuerySystemInformation - %d, %d", psi->Handles[i].ProcessId, GetCurrentProcessId());
#endif
					psi->Handles[i].GrantedAccess = 0;
					psi->Handles[i].ProcessId = GetCurrentProcessId();
					psi->Handles[i].Flags = 0;
					psi->Handles[i].Handle = 0;
					psi->Handles[i].Object = 0;
					psi->Handles[i].ObjectTypeNumber = 0;

				}
			}
			break;
		case 123:
			break;
		case 134:
			break;
		default:
			break;
		}
	}
	VirtualizerEnd();
	return Status;
}

NTSTATUS NTAPI Bypass::NtReadVirtualMemory_Hook(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesReaded)
{
	VirtualizerStart();
	CloseHandle(ProcessHandle);
	VirtualizerEnd();
	return true;
}

NTSTATUS NTAPI Bypass::NtGetContextThread_Hook(HANDLE ThreadHandle, PCONTEXT pContext)
{
	VirtualizerStart();
	HMODULE wModule = 0;
	NTSTATUS Status = o_NtGetContextThread(ThreadHandle, pContext);
	if (NT_SUCCESS(Status) && pContext)
	{
#if (LOG_NTGETCONTEXTTHREAD == 1)
		DbgLog::Log("NtGetContextThread - %d, 0x%x", GetThreadId(ThreadHandle), pContext->Rip);
#endif
		if (pContext->Dr0)
			pContext->Dr0 = 0;
		if (pContext->Dr1)
			pContext->Dr1 = 0;
		if (pContext->Dr2)
			pContext->Dr2 = 0;
		if (pContext->Dr3)
			pContext->Dr3 = 0;
		if (pContext->Dr7)
			pContext->Dr7 = 0;
	}
	VirtualizerEnd();
	return Status;
}

NTSTATUS NTAPI Bypass::NtOpenThread_Hook(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PVOID Inject_GameId)
{
	VirtualizerStart();
	typedef struct _CLIENT_ID {
		HANDLE UniqueProcess;
		HANDLE UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;

	HMODULE wModule = 0;
	PCLIENT_ID Inject_GameID = reinterpret_cast<PCLIENT_ID>(Inject_GameId);
#if (LOG_NTOPENTHREAD == 1)
	DbgLog::Log("NtOpenThread - %d, 0x%x", Inject_GameID->UniqueThread, 0);
#endif
	// TerminateThread(GetCurrentThread(), 0);
	DesiredAccess = THREAD_TERMINATE;
	VirtualizerEnd();
	return o_NtOpenThread(ThreadHandle, DesiredAccess, ObjectAttributes, Inject_GameId);
}

NTSTATUS NTAPI Bypass::NtCreateFile_Hook(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength)
{
	VirtualizerStart();
	DWORD dwWritten = 0;
	NTSTATUS Status = -1;
	if (ObjectAttributes &&
		ObjectAttributes->ObjectName &&
		ObjectAttributes->ObjectName->Buffer)
	{
		if (wcsstr(ObjectAttributes->ObjectName->Buffer, L"Bypass")) {

#if (LOG_NTCREATEFILE == 1)
			DbgLog::Log("NtCreateFile - %ls", ObjectAttributes->ObjectName->Buffer);
#endif
			o_RtlInitUnicodeString(ObjectAttributes->ObjectName, L"");
			NTSTATUS Status = o_NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
			return Status;
		}

	}
	VirtualizerEnd();
	return o_NtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);

};


#pragma endregion
