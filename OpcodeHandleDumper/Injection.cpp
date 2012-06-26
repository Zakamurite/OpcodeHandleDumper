#include "Injection.h"
#include "Log.h"

BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved) 
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        moduleName = "Wow_strawberry.exe";
        DefineOffsets();
        InstallPatches();
        Log("Patches installed");
        DumpHandlers();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Log("Dll detached");
        RemovePatches();
    }

    return 1;
}