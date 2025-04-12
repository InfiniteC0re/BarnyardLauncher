#include "pch.h"
#include "Utils.h"

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

void Platform_OpenInShell( const char* path )
{
	::ShellExecuteA( NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT );
}
