// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

#include <iostream>
#include <ctime>
#include <sys/timeb.h>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "log.hpp"

#if defined(_WIN32)
#include <windows.h>
#endif

FILE *log = NULL;

void LogPrintf(int Level, const char *format, ...)
{
	va_list args;
	char buffer[2048];
	
	va_start(args, format);
	std::vsnprintf(buffer, 2047, format, args);
	va_end(args);
	
	if (log == NULL) {
		log = std::fopen("libm3.log", "w");
	}
	
	// Create log message
	std::string Log = StringFromFormat("%s %s", GetTimeFormatted().c_str(), buffer);
	fwrite(Log.c_str(), 1, strlen(Log.c_str()), log);
	fflush(log);
	LogConsole(Level, Log);
}


#if defined(_WIN32)
void LogConsole(int Level, std::string s)
{
	HWND hWnd = GetConsoleWindow();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!hWnd)
	{
		// Create console
		AllocConsole();	
		SetConsoleTitleA("libm3");
		// Set console size
		int iWidth = 120, iHeight = 50;
		COORD Co = {iWidth, 1000};
		BOOL SB = SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), Co);
		MoveWindow(GetConsoleWindow(), 0,0, 3000*8,iHeight*14, true);
	}

	WORD Color;
	switch (Level)
	{
	case LYELLOW: // light yellow
		Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case LGREEN: // light green
		Color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	default: // off-white
		Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	}

	SetConsoleTextAttribute(hConsole, Color);
	DWORD cCharsWritten;
	if(hConsole)
		WriteConsoleA(hConsole, s.c_str(), strlen(s.c_str()), &cCharsWritten, NULL);
}
#else
void LogConsole(int Level, std::string s) {
    std::cout << s << std::endl;
}
#endif


// Return the current time formatted as Minutes:Seconds:Milliseconds in the form 00:00:000
std::string GetTimeFormatted()
{
	time_t sysTime;
	struct tm * gmTime = NULL;
	char formattedTime[13];
	char tmp[13];

	time(&sysTime);
    gmTime = std::localtime(&sysTime);

	strftime(tmp, 6, "%M:%S", gmTime);

	// Now add the milliseconds
	struct timeb tp;
	(void)::ftime(&tp);
	sprintf(formattedTime, "%s:%03i", tmp, tp.millitm);

	return std::string(formattedTime);
}

// Printf to string
std::string StringFromFormat(const char* format, ...)
{
	int writtenCount = -1;
	int newSize = (int)strlen(format) + 4;
	char *buf = 0;
	va_list args;
	while (writtenCount < 0)
	{
		delete [] buf;
		buf = new char[newSize + 1];
	
	    va_start(args, format);
		writtenCount = std::vsnprintf(buf, newSize + 1, format, args);
		va_end(args);
		if (writtenCount >= (int)newSize) {
			writtenCount = -1;
		}
		newSize *= 2;
	}

	buf[writtenCount] = '\0';
	std::string temp = buf;
	return temp;
}
