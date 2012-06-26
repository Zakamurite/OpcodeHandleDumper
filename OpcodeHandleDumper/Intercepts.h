#ifndef _INTERCEPTS_H
#define _INTERCEPTS_H

void ProcessNormalHandlerIntercept();
void __stdcall OnProcessNormalHandlerIntercept(unsigned int handler, unsigned int opcode);

#endif
