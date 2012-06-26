#include "Log.h"
#include "Opcodes.h"
#include "Ptrs.h"
#include <Windows.h>

bool AuthCheck(unsigned int opcode)
{
    return (opcode & 0xB3FD) == 320;
}

bool SpecialCheck(unsigned int opcode)
{
    return (opcode & 0x92E8) == 4256;
}

bool NormalCheck(unsigned int opcode)
{
    return (opcode & 0x90CC) == 4;
}

// handler id in ClientConnection.Handlers array
unsigned int CalcCryptedFromOpcode(unsigned int opcode)
{
    return opcode & 3 | ((opcode & 0x30 | ((opcode & 0xF00 | (opcode >> 1) & 0x3000) >> 2)) >> 2);
}

// switch value in NetClient__JAMClientDispatch
unsigned int CalcSpecialFromOpcode(unsigned int opcode)
{
    return (opcode & 7 | ((opcode & 0x10 | ((opcode & 0x100 | ((opcode & 0xC00 | (opcode >> 1) & 0x3000) >> 1)) >> 3)) >> 1));
}

// switch value in NetClient__JAMClientConnectionDispatch
unsigned int CalcAuthFromOpcode(unsigned int opcode)
{
    return (opcode & 2 | ((opcode & 0xC00 | (opcode >> 2) & 0x1000) >> 8)) >> 1;
}

void DumpHandlers()
{
    unsigned int wowOffset =  (unsigned int)GetModuleHandleA(moduleName);
    for (unsigned int op = 0; op < MAX_OPCODE; ++op)
    {
        if (AuthCheck(op))
        {
            unsigned int auth = CalcAuthFromOpcode(op);
            Log("Opcode: %u (0x%X), type: auth, offset: 0x%X", op, op, auth);
        }
        else if (SpecialCheck(op))
        {
            unsigned int crypted = CalcSpecialFromOpcode(op);
            Log("Opcode: %u (0x%X), type: special, offset: 0x%X", op, op, crypted);
        }
        else if (NormalCheck(op))
        {
        }
        else
        {
            //Log("Opcode: %u, unknown type.", op);
        }
    }
}
