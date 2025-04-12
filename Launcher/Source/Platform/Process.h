#pragma once
#include "System.h"

#include <cstdint>
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>

class Process
{
public:
	typedef uint32_t Priority;
	enum Priority_ : Priority
	{
		Priority_Idle,
		Priority_BelowNormal,
		Priority_Normal,
		Priority_AboveNormal,
		Priority_High,
		Priority_RealTime,
	};

	using ProcessInfo = PROCESS_INFORMATION;

public:
	Process()
	{
		m_hProcess = INVALID_HANDLE_VALUE;
	}

	Process( HANDLE hProcess )
	    : m_hProcess( hProcess )
	{
	}

	Process( Process&& other ) noexcept
	{
		m_hProcess       = other.m_hProcess;
		other.m_hProcess = INVALID_HANDLE_VALUE;
	}

	Process( const Process& ) = delete;

	~Process()
	{
		CloseHandle();
	}

	HANDLE GetHandle() const
	{
		return m_hProcess;
	}

	DWORD GetId() const
	{
		return GetProcessId( m_hProcess );
	}

	HANDLE Release()
	{
		HANDLE hProcess = GetHandle();
		m_hProcess      = INVALID_HANDLE_VALUE;
		return hProcess;
	}

	void CloseHandle()
	{
		if ( m_hProcess != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( m_hProcess );
			m_hProcess = INVALID_HANDLE_VALUE;
		}
	}

public:
	static bool Create( const wchar_t* process, const wchar_t* commandLine = NULL, const wchar_t* currentDirectory = NULL, Priority priority = Priority_Normal, int numCores = 1, ProcessInfo* pProcessInfo = NULL )
	{
		DWORD creationFlags = 0;

		switch ( priority )
		{
			case Priority_Idle:
				creationFlags |= IDLE_PRIORITY_CLASS;
				break;
			case Priority_BelowNormal:
				creationFlags |= BELOW_NORMAL_PRIORITY_CLASS;
				break;
			case Priority_Normal:
				creationFlags |= NORMAL_PRIORITY_CLASS;
				break;
			case Priority_AboveNormal:
				creationFlags |= ABOVE_NORMAL_PRIORITY_CLASS;
				break;
			case Priority_High:
				creationFlags |= HIGH_PRIORITY_CLASS;
				break;
			case Priority_RealTime:
				creationFlags |= REALTIME_PRIORITY_CLASS;
				break;
		}

		STARTUPINFOW startupInfo;
		memset( &startupInfo, '\0', sizeof( startupInfo ) );

		PROCESS_INFORMATION processInfo;
		BOOL                bRes = CreateProcessW( process, (LPWSTR)commandLine, NULL, NULL, FALSE, creationFlags, NULL, currentDirectory, &startupInfo, &processInfo );

		if ( bRes )
		{
			if ( pProcessInfo != NULL )
			{
				*pProcessInfo = processInfo;
			}

			if ( numCores == -1 )
			{
				numCores = System::GetNumberOfProcessors();
			}

			DWORD_PTR processAffinityMask = ( 1 << numCores ) - 1;
			SetProcessAffinityMask( processInfo.hProcess, processAffinityMask );
			::CloseHandle( processInfo.hProcess );
			::CloseHandle( processInfo.hThread );

			return true;
		}

		return false;
	}

	static std::vector<Process> FindProcessesByName( wchar_t* name )
	{
		PROCESSENTRY32W entry;
		entry.dwSize = sizeof( entry );

		std::vector<Process> processes;
		HANDLE               hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

		if ( Process32FirstW( hSnapshot, &entry ) == TRUE )
		{
			while ( Process32NextW( hSnapshot, &entry ) == TRUE )
			{
				if ( wcscmp( entry.szExeFile, name ) == 0 )
				{
					HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID );
					processes.emplace_back( hProcess );
				}
			}
		}

		::CloseHandle( hSnapshot );

		return processes;
	}

private:
	HANDLE m_hProcess;
};
