#include "pch.h"
#include "Hash.h"

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

TUINT32 Hash_Vec2i( TINT a_iX, TINT a_iY )
{
	TINT buffer[] = { a_iX, a_iY };
	return TUtil::CRC32( buffer, TARRAYSIZE( buffer ) );
}
