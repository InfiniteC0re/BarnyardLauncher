#pragma once
#include <Windows.h>

class System
{
public:
	static unsigned long GetNumberOfProcessors()
	{
		static unsigned long s_CachedResult = -1;

		if ( s_CachedResult == -1 )
		{
			SYSTEM_INFO info;
			GetSystemInfo( &info );
			s_CachedResult = info.dwNumberOfProcessors;
		}

		return s_CachedResult;
	}
};
