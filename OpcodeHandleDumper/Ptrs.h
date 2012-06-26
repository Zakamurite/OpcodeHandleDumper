#ifdef _DEFINE_VARS

#define DLLOFFSET(a1) (a1)
#define FUNCPTR(v1,t1,t2,o1)    typedef t1 WOW_##v1##_t t2; WOW_##v1##_t *WOW_##v1 = (WOW_##v1##_t *)DLLOFFSET(o1);
#define VARPTR(v1,t1,o1)        typedef t1 WOW_##v1##_t;    WOW_##v1##_t *p_##d1##_##v1 = (WOW_##v1##_t *)DLLOFFSET(o1);
#define ASMPTR(v1,o1)           DWORD WOW_##v1 = DLLOFFSET(o1);

char* moduleName;

#else

#define FUNCPTR(v1,t1,t2,o1)    typedef t1 WOW_##v1##_t t2; extern WOW_##v1##_t *WOW_##v1;
#define VARPTR(v1,t1,o1)        typedef t1 WOW_##v1##_t;    extern WOW_##v1##_t *p_WOW_##v1;
#define ASMPTR(v1,o1)           extern DWORD WOW_##v1;

extern char* moduleName;
#endif

#define _WOWPTRS_START  WOW_NetClient__ProcessMessage

FUNCPTR(NetClient__ProcessMessage, char, (unsigned int * _this, unsigned int a2, unsigned int * opcode, unsigned int a4), 0x86FA0 + 0x1000)
FUNCPTR(ClientServices__Connection, unsigned int * __cdecl, (void), 0xCDA40 + 0x1000)

#define _WOWPTRS_END    WOW_ClientServices__Connection
