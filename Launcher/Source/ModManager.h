#pragma once
#include <ToshiTools/T2DynamicVector.h>
#include <Toshi/TString8.h>
#include <Toshi/TString16.h>
#include <Toshi/TVersion.h>

namespace ModManager
{

struct Mod
{
	Toshi::TString8 strDllPath;
	Toshi::TString8 strName;
	Toshi::TString8 strAutoUpdateURL;
	Toshi::TVersion uiVersion;
	TBOOL           bAutoUpdates;
};

void                         Initialise();
void                         ScanForMods();
void                         CheckForUpdates();
Toshi::T2DynamicVector<Mod>& GetMods();

}