#include "pch.h"
#include "GameSettings.h"
#include "vdf.hpp"

#include <Toshi/T2String.h>
#include <Plugins/PTRB.h>

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

using namespace tyti;

TOSHI_NAMESPACE_USING

GameSettings::GameSettings()
{
}

GameSettings::~GameSettings()
{
}

void GameSettings::Save()
{
	std::ofstream fileStream( "Launcher\\BYLauncher.vdf" );

	vdf::object settings;
	settings.set_name( "BYardLauncher" );
	settings.attribs[ "experimental" ] = vdf::toString( bExperimental );
	settings.attribs[ "fun" ]          = vdf::toString( bFun );
	settings.attribs[ "dxvk" ]         = vdf::toString( bDXVK );
	settings.attribs[ "windowed" ]     = vdf::toString( bWindowed );
	settings.attribs[ "width" ]        = vdf::toString( iWidth );
	settings.attribs[ "height" ]       = vdf::toString( iHeight );

	vdf::write( fileStream, settings );

	TINFO( "Saved core settings file\n" );
}

void GameSettings::Load()
{
	std::ifstream fileStream( "Launcher\\BYLauncher.vdf" );

	if ( !fileStream.is_open() )
		return;

	vdf::object settings = vdf::read( fileStream );

	if ( settings.name != "BYardLauncher" )
		return;

	bExperimental = vdf::getBool( settings, "experimental", TFALSE );
	bFun          = vdf::getBool( settings, "fun", TFALSE );
	bDXVK         = vdf::getBool( settings, "dxvk", TTRUE );
	bWindowed     = vdf::getBool( settings, "windowed", TFALSE );
	iWidth        = vdf::getInt( settings, "width", 0 );
	iHeight       = vdf::getInt( settings, "height", 0 );

	Apply();
}

void GameSettings::Apply()
{
}
