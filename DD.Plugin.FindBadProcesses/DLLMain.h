#include <Windows.h>
#include <tlhelp32.h>
#include <vector>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) TCHAR* __cdecl PluginName(void);
	__declspec(dllexport) TCHAR* __cdecl PluginVersion(void);
	__declspec(dllexport) TCHAR* __cdecl PluginErrorMessage(void);
	__declspec(dllexport) DWORD __cdecl PluginDebugCheck(void);

#ifdef __cplusplus
}
#endif

TCHAR* sErrorMessage;