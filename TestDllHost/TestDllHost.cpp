// TestDllHost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>



BOOL CALLBACK EnumWindowsProc(
	_In_ HWND   hWnd,
	_In_ LPARAM lParam
) {
	printf("EnumWindowsProc hWnd: %08p\n", hWnd);
	return TRUE;
}



int main()
{
	getchar();
	EnumWindows(EnumWindowsProc, NULL);
	getchar();
	printf("Hello World!\n");
}
