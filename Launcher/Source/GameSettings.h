#pragma once

struct GameSettingsProperties
{
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
