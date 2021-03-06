// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <winternl.h>
#include <detours.h>
#include <TlHelp32.h>
#include <intrin.h>
#include <stdio.h>
#include <tchar.h>
#pragma comment(lib,"detours.lib")

#define USE_VMPROTECT 0

#define DISABLE_DLLDETECTION 1
#define DISABLE_THREADDETECTION 1
#define DISABLE_PROCESSDETECTION 1
#define DISABLE_ANTIDEBUGGER 1
#define DISABLE_READFILE 1 // unused

#define LOG_STATE 1
#define LOG_CREATESEMPAPHORE 1
#define LOG_NTQUERYVIRTUALMEMORY 1
#define LOG_NTWOW64QUERYVIRTUALMEMORY64 1
#define LOG_ENUMWINDOWS 1
#define LOG_NTQUERYWINDOW 1
#define LOG_NTUSERWINDOWFUNCTION 1
#define LOG_NTOPENPROCESS 1
#define LOG_NTQUERYSYSTEMINFORMATION 1
#define LOG_NTGETCONTEXTTHREAD 1
#define LOG_NTSETINFORMATIONTHREAD 1
#define LOG_NTOPENTHREAD 1
#define LOG_ZWREADFILE 1
#define LOG_NTCREATEFILE 1

#include "NtHelper.h"
#include "Virtualizer.hpp"
#include "DbgLog.hpp"
#include "ModuleHidden.hpp"