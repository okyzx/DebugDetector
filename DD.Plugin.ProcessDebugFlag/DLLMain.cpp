#include "DLLMain.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	return true;
}

__declspec(dllexport) TCHAR* __cdecl PluginName(void)
{
	return L"ProcessDebugFlags";
}

__declspec(dllexport) TCHAR* __cdecl PluginVersion(void)
{
	return L"0.1";
}

__declspec(dllexport) TCHAR* __cdecl PluginErrorMessage(void)
{
	return sErrorMessage;
}

__declspec(dllexport) DWORD __cdecl PluginDebugCheck(void)
{
	typedef NTSTATUS (WINAPI *pNtQueryInformationProcess)(HANDLE ,UINT ,PVOID ,ULONG , PULONG); 

	DWORD NoDebugInherit = 0; 
	NTSTATUS Status; 

	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"NtQueryInformationProcess"); 

	Status = NtQIP(GetCurrentProcess(),0x1f,&NoDebugInherit,4,NULL); 

	if (Status != 0x00000000)
	{
		sErrorMessage = (TCHAR*)malloc(255);
		wsprintf(sErrorMessage,L"0x%08",Status);
		return -1; 
	}

	if(NoDebugInherit == FALSE)
		return 1;
	else
		return 0;
}