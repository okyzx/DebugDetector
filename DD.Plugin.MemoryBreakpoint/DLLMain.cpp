#include "DLLMain.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	return true;
}

__declspec(dllexport) TCHAR* __cdecl PluginName(void)
{
	return L"PageGuard Check";
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
	unsigned char *pMem = NULL;
	SYSTEM_INFO sysinfo = {0}; 
	DWORD OldProtect = 0;
	void *pAllocation = NULL;

	GetSystemInfo(&sysinfo);

	pAllocation = VirtualAlloc(NULL,sysinfo.dwPageSize,MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE); 

	if (pAllocation == NULL)
	{
		sErrorMessage = (TCHAR*)malloc(255);
		wsprintf(sErrorMessage,L"%s","Alloc failed!");
		return -1; 
	}

	pMem = (unsigned char*)pAllocation;
	*pMem = 0xc3; // ret
       
	if (VirtualProtect(pAllocation, sysinfo.dwPageSize,PAGE_EXECUTE_READWRITE | PAGE_GUARD,&OldProtect) == 0)
		return false;

	__try
	{
		__asm
		{
			mov eax, pAllocation
			push MemBpBeingDebugged
			jmp eax
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		VirtualFree(pAllocation, NULL, MEM_RELEASE);
		return 0;
	}     

	__asm{MemBpBeingDebugged:}
	VirtualFree(pAllocation, NULL, MEM_RELEASE);
	return 1;
}