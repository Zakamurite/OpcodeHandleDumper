#include "Log.h"
#include "Ptrs.h"
#include "Intercepts.h"
#include <Windows.h>

void __declspec(naked) ProcessNormalHandlerIntercept()
{
    unsigned int returnaddr;
    __asm
    {
        pop returnaddr
        pushad
        push esi
        push ecx
        call OnProcessNormalHandlerIntercept;
        popad
        mov eax, 0  // call ecx
        add esp, 10h
        push returnaddr
        retn
    }
}

void __stdcall OnProcessNormalHandlerIntercept(unsigned int handler, unsigned int opcode)
{
    unsigned int wowOffset =  (unsigned int)GetModuleHandleA(moduleName);
    Log("Opcode: %u (0x%X), type: normal, offset: 0x%X", opcode, opcode, handler - wowOffset + 0x1000);
}
