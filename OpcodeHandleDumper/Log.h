#ifndef _LOG_H
#define _LOG_H

enum Logtype
{
    LOG_DEBUG,
    LOG_ERROR,
    LOG_REGULAR,    
};

void Log(Logtype lvl, char* format, ...);
void Log(char* format, ...);

#endif
