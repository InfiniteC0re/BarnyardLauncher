#include "pch.h"
#include "GameSettings.h"

#include <Toshi/T2String.h>
#include <Plugins/PTRB.h>

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

GameSettings::GameSettings()
{
}

GameSettings::~GameSettings()
{
}


void GameSettings::Save()
{
	PTRB                        outFile;
	PTRBSections*               pSections  = outFile.GetSections();
	PTRBSymbols*                pSymbols   = outFile.GetSymbols();
	PTRBSections::MemoryStream* pMemStream = pSections->CreateStream();

	// Write settings to the stream
	auto pSettings = pMemStream->Alloc<GameSettingsProperties>();
	*pSettings     = *this;

	// Save the file
	pSymbols->Add( pMemStream, "Settings", pSettings.get() );
	outFile.WriteToFile( "BYLauncher.trb" );

	TINFO( "Saved core settings file\n" );
}

void GameSettings::Load()
{
	TTRB inFile;

	if ( inFile.Load( "BYLauncher.trb" ) == TTRB::ERROR_OK )
	{
		TINFO( "Reading core settings file...\n" );
		GameSettingsProperties* pSettings = inFile.CastSymbol<GameSettingsProperties>( "Settings" );

		if ( pSettings->uiMagic != TFourCC( "BYLN" ) )
		{
			TERROR( "The settings file has wrong signature!\n" );
			return;
		}

		switch ( pSettings->uiVersion )
		{
			case 1:
				bExperimental = pSettings->bExperimental;
				bFun          = pSettings->bFun;
				bDXVK         = pSettings->bDXVK;
				bWindowed     = pSettings->bWindowed;
				iWidth        = pSettings->iWidth;
				iHeight       = pSettings->iHeight;
				break;
			default:
				TINFO( "The settings file's version is not supported!\n" );
				break;
		}
	}

	Apply();
}

void GameSettings::Apply()
{
	
}
