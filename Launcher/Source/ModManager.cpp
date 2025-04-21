#include "pch.h"
#include "ModManager.h"

// We don't need any other header files of the ModCore to be in the list of includes
#include <../../OpenBarnyard/SDK/BYModCore/Include/ModLoader.h>

#include <ToshiTools/json.hpp>

#include <curl/curl.h>
#include <filesystem>

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

static CURL*                            s_pCurl;
static T2DynamicVector<ModManager::Mod> s_vecMods;

static size_t CURLWriteCallback( void* contents, size_t size, size_t nmemb, void* userp )
{
	( (std::string*)userp )->append( (char*)contents, size * nmemb );
	return size * nmemb;
}

void ModManager::Initialise()
{
	curl_global_init( CURL_GLOBAL_DEFAULT );

	s_pCurl = curl_easy_init();

	if ( !s_pCurl )
	{
		TERROR( "curl_easy_init() returned null pointer\n" );
		return;
	}

	curl_easy_setopt( s_pCurl, CURLOPT_SSL_VERIFYPEER, 0L );
	curl_easy_setopt( s_pCurl, CURLOPT_SSL_VERIFYHOST, 0L );
	curl_easy_setopt( s_pCurl, CURLOPT_CA_CACHE_TIMEOUT, 604800L );
	curl_easy_setopt( s_pCurl, CURLOPT_WRITEFUNCTION, CURLWriteCallback );
	curl_easy_setopt( s_pCurl, CURLOPT_FOLLOWLOCATION, TRUE );
}

void ModManager::ScanForMods()
{
	for ( const auto& entry : std::filesystem::directory_iterator( "Mods\\" ) )
	{
		if ( entry.path().extension().compare( L".dll" ) == 0 )
		{
			HMODULE hModule               = LoadLibraryExW( entry.path().native().c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES );
			auto    fnGetModAutoUpdateURL = TREINTERPRETCAST( t_GetModAutoUpdateURL, GetProcAddress( hModule, "GetModAutoUpdateURL" ) );
			auto    fnGetVersion          = TREINTERPRETCAST( t_GetModVersion, GetProcAddress( hModule, "GetModVersion" ) );
			auto    fnGetName             = TREINTERPRETCAST( t_GetModName, GetProcAddress( hModule, "GetModName" ) );

			// Obtain the update url
			TString8 updateInfoURL = ( fnGetModAutoUpdateURL ) ? fnGetModAutoUpdateURL() : "";
			TString8 strModName    = ( fnGetName ) ? fnGetName() : "Unknown Mod";
			TVersion modVersion    = ( fnGetVersion ) ? fnGetVersion() : TVERSION( 1, 0 );

			// We don't need the dll to be attached anymore
			FreeLibrary( hModule );

			// Add info about the mod
			s_vecMods.PushBack();
			auto mod              = s_vecMods.Back();
			mod->strName          = strModName;
			mod->strAutoUpdateURL = updateInfoURL;
			mod->strDllPath       = Toshi::Platform_UnicodeToUTF8( entry.path().native().c_str() );
			mod->uiVersion        = modVersion;
			mod->bAutoUpdates     = updateInfoURL && fnGetVersion;

			//UpdateManager::VersionInfo versionInfo;
			//TBOOL                      bOutDated = UpdateManager::CheckVersion( updateInfoURL.GetString(), modVersion, &versionInfo );

			//if ( bOutDated )
			//{
			//	TINFO( "New version (v%u.%u) of the '%s' (v%u.%u) mod is available\n", versionInfo.uiVersion.Parts.Major, versionInfo.uiVersion.Parts.Minor, strModName.GetString(), modVersion.Parts.Major, modVersion.Parts.Minor );

			//	TString8 strDescription = TString8::VarArgs(
			//	    "New version of the '%s' mod is available!\n\n"
			//	    "Current version: v%u.%u\n"
			//	    "Latest version:  v%u.%u\n\n"
			//	    "Would you like to update now?",
			//	    strModName.GetString(),
			//	    modVersion.Parts.Major,
			//	    modVersion.Parts.Minor,
			//	    versionInfo.uiVersion.Parts.Major,
			//	    versionInfo.uiVersion.Parts.Minor
			//	);

			//	const TINT msgBoxResult = MessageBoxA( NULL, strDescription.GetString(), "Barnyard ModLoader", MB_YESNO );

			//	if ( msgBoxResult == IDYES )
			//	{
			//		TINFO( "User agreed to update mod\n" );

			//		AHTTPClient httpClient;
			//		httpClient.Create( versionInfo.strUpdateUrl.GetString() );
			//		httplib::Result downloadResult = httpClient.Get();

			//		// Redirect once
			//		if ( downloadResult->get_header_value_count( "location" ) )
			//		{
			//			httpClient.Create( downloadResult->get_header_value( "location" ).c_str() );
			//			downloadResult = httpClient.Get();
			//		}

			//		if ( downloadResult.error() != httplib::Error::Success ||
			//		     downloadResult->status != 200 )
			//		{
			//			TERROR( "Unable to download new version of the mod\n" );
			//			TASSERT( TFALSE );
			//			MessageBoxA( NULL, "Unable to download new version of the mod! Canceling...", "Barnyard ModLoader", MB_OK );
			//			continue;
			//		}

			//		TINFO( "Downloaded new version of the mod!\n" );

			//		HANDLE hFile = ::CreateFileW( entry.path().native().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			//		if ( hFile == INVALID_HANDLE_VALUE )
			//		{
			//			TERROR( "Unable to open file for writing. Error: %u\n", GetLastError() );
			//			TASSERT( TFALSE );
			//			MessageBoxA( NULL, "Unable to open file for writing! Canceling...", "Barnyard ModLoader", MB_OK );
			//			continue;
			//		}

			//		DWORD dwNumWritten;
			//		BOOL  bWritten = WriteFile( hFile, downloadResult->body.c_str(), downloadResult->body.size(), &dwNumWritten, NULL );

			//		if ( !bWritten || dwNumWritten != downloadResult->body.size() )
			//		{
			//			TERROR( "An unknown error has occured during updating the mod. Error: %u\n", GetLastError() );
			//			TASSERT( TFALSE );
			//			MessageBoxA( NULL, "An unknown error has occured during updating the mod.", "Barnyard ModLoader", MB_OK );
			//		}

			//		::CloseHandle( hFile );

			//		TINFO( "Successfully updated the mod!\n" );
			//		MessageBoxA( NULL, "The mod was successfully updated!", "Barnyard ModLoader", MB_OK );
			//	}
			//}
		}
	}
}

struct VersionInfo
{
	Toshi::TVersion uiVersion;
	Toshi::TString8 strUpdateUrl;
};

static TBOOL CheckVersion( Toshi::T2StringView strUpdateInfoUrl, Toshi::TVersion uiCurrentVersion, VersionInfo* pOutVersionInfo )
{
	std::string responseBuffer;
	curl_easy_setopt( s_pCurl, CURLOPT_WRITEDATA, &responseBuffer );
	curl_easy_setopt( s_pCurl, CURLOPT_URL, strUpdateInfoUrl.Get() );

	// Do the request
	CURLcode res = curl_easy_perform( s_pCurl );
	if ( res != CURLE_OK ) return TFALSE;

	// Parse json
	nlohmann::json json;
	try
	{
		json = json.parse( responseBuffer );
	}
	catch (...)
	{
		TERROR( "Error parsing JSON response from URL: %s\n", strUpdateInfoUrl.Get() );
		return TFALSE;
	}

	if ( json.is_null() || !json.is_object() )
		return TFALSE;

	auto version = json.find( "version" );
	auto latest  = json.find( "latest" );

	if ( version == json.end() || latest == json.end() ) return TFALSE;
	if ( !version->is_array() || !latest->is_string() ) return TFALSE;

	if ( version->size() != 2 || !version->at( 0 ).is_number_integer() || !version->at( 1 ).is_number_integer() )
		return TFALSE;

	TVersion latestVersion = TVERSION( version->at( 0 ).get<TINT>(), version->at( 1 ).get<TINT>() );

	if ( pOutVersionInfo )
	{
		pOutVersionInfo->uiVersion    = latestVersion;
		pOutVersionInfo->strUpdateUrl = latest->get<std::string>().c_str();
	}

	return ( uiCurrentVersion.Parts.Major < latestVersion.Parts.Major || uiCurrentVersion.Parts.Minor < latestVersion.Parts.Minor );
}


void ModManager::CheckForUpdates()
{
	TINFO( "Ckecking for updates...\n" );

	T2_FOREACH( s_vecMods, it )
	{
		if ( !it->bAutoUpdates )
			continue;
		
		VersionInfo versionInfo;
		CheckVersion( it->strAutoUpdateURL.GetString(), it->uiVersion, &versionInfo );
	}
}

T2DynamicVector<ModManager::Mod>& ModManager::GetMods()
{
	return s_vecMods;
}
