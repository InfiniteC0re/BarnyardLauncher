#pragma once

struct GameSettingsProperties
{
	// Common for all versions:
	TUINT uiMagic   = TFourCC( "BYLN" );
	TUINT uiVersion = 1;

	// Version 1:
	TBOOL bExperimental = TFALSE;
	TBOOL bFun          = TFALSE;
	TBOOL bWindowed     = TFALSE;
	TBOOL bDXVK         = TTRUE;
	TINT  iWidth        = 0;
	TINT  iHeight       = 0;
};

inline struct GameSettings
    : public GameSettingsProperties
{

	GameSettings();
	~GameSettings();

	void Save();
	void Load();

	void Apply();

} g_oSettings;
