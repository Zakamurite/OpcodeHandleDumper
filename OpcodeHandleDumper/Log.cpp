#include "Log.h"
#include "stdio.h"
#include <windows.h>

void Log(char* format, ...)
{
    char text[4096];
	
	va_list arguments;
	va_start(arguments, format);
	vsprintf_s(text, format, arguments);
	va_end(arguments);

    Log(LOG_REGULAR, text);
}

void Log(Logtype type, char* format, ...)
{
	char text[4096];

	va_list arguments;
	va_start(arguments, format);
	vsprintf_s(text, format, arguments);
	va_end(arguments);

	SYSTEMTIME t;
	char timebuf[256];
	GetLocalTime(&t);
	sprintf_s(timebuf, sizeof(timebuf), "[%04d-%02d-%02d %02d:%02d:%02d] ", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

	FILE* fp;
    fopen_s(&fp, "dumper.log", "a+");

	fseek(fp, 0, SEEK_END);
	fwrite(timebuf, strlen(timebuf), 1, fp);

    switch(type)
    {
        case LOG_DEBUG: fwrite("Debug: ", 7, 1, fp);    break;
        case LOG_ERROR: fwrite("Error: ", 7, 1, fp);    break;
    }

	fwrite(text, strlen(text), 1, fp);
	fwrite("\n", 1, 1, fp);
	fflush(fp);
	fclose(fp);
}