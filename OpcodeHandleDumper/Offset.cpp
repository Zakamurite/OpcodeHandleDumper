#define _DEFINE_VARS

#include "Injection.h"
#include "Log.h"

PatchHook Patches[4];
int PatchCount = 0;

void LoadPatches()
{
    // .text:0048747C FF D1                             call    ecx
    Patches[PatchCount].pFunc = PatchCall;
    Patches[PatchCount].dwAddr = GetDllOffset(moduleName, 0x8647C + 0x1000);
    Patches[PatchCount].dwFunc = (DWORD)ProcessNormalHandlerIntercept;
    Patches[PatchCount].dwLen = 5;
    Log("Patch %u addr: 0x%X", PatchCount, Patches[PatchCount].dwAddr);
    ++PatchCount;
}

void DefineOffsets()
{
    DWORD* p = (DWORD *)&_WOWPTRS_START;
    do
    {
        *p = GetDllOffset(*p);
    }
    while (++p <= (DWORD *)&_WOWPTRS_END);
}

DWORD GetDllOffset(char* DllName, int Offset)
{
    HMODULE hMod = GetModuleHandleA(DllName);

    if (!hMod)
        hMod = LoadLibraryA(DllName);

    if (!hMod)
        return 0;

    if (Offset < 0)
        return (DWORD)GetProcAddress(hMod, (LPCSTR)(-Offset));
    
    return ((DWORD)hMod) + Offset;
}

DWORD GetDllOffset(int num)
{
    return GetDllOffset(moduleName, num);
}

void InstallPatches()
{    
    LoadPatches();
    for (int x = 0; x < PatchCount; ++x)
    {
        Patches[x].bOldCode = new BYTE[Patches[x].dwLen];
        ::ReadProcessMemory(GetCurrentProcess(), (void*)Patches[x].dwAddr, Patches[x].bOldCode, Patches[x].dwLen, NULL);
        Patches[x].pFunc(Patches[x].dwAddr, Patches[x].dwFunc, Patches[x].dwLen);
    }    
}

void RemovePatches()
{
    for (int x = 0; x < PatchCount; x++)
    {
        WriteBytes((void*)Patches[x].dwAddr, Patches[x].bOldCode, Patches[x].dwLen);
        delete Patches[x].bOldCode;
    }    
}

BOOL WriteBytes(void *pAddr, void *pData, DWORD dwLen)
{
    DWORD dwOld;

    if (!VirtualProtect(pAddr, dwLen, PAGE_READWRITE, &dwOld))
        return FALSE;

    ::memcpy(pAddr, pData, dwLen);
    return VirtualProtect(pAddr, dwLen, dwOld, &dwOld);
}

void FillBytes(void *pAddr, BYTE bFill, DWORD dwLen)
{
    BYTE *bCode = new BYTE[dwLen];
    ::memset(bCode, bFill, dwLen);

    WriteBytes(pAddr, bCode, dwLen);

    delete bCode;
}

void InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen)
{
    BYTE *bCode = new BYTE[dwLen];
    ::memset(bCode, 0x90, dwLen);
    DWORD dwFunc = pFunc - (pAddr + 5);

    bCode[0] = bInst;
    *(DWORD *)&bCode[1] = dwFunc;
    WriteBytes((void*)pAddr, bCode, dwLen);

    delete bCode;
}

void PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen)
{
    InterceptLocalCode(INST_CALL, dwAddr, dwFunc, dwLen);
}

void PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen)
{
    InterceptLocalCode(INST_JMP, dwAddr, dwFunc, dwLen);
}

void PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen)
{
    BYTE *bCode = new BYTE[dwLen];
    ::memset(bCode, (BYTE)dwValue, dwLen);

    WriteBytes((LPVOID)dwAddr, bCode, dwLen);

    delete bCode;
}

PatchHook *RetrievePatchHooks(PINT pBuffer)
{
    *pBuffer = ArraySize(Patches);
    return &Patches[0];
}
