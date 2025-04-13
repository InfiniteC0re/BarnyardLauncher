#include "pch.h"
#include "Application.h"
#include "ModManager.h"
#include "Screens/UIScreenControl.h"
#include "Screens/MainScreen.h"

#include "Platform/Utils.h"
#include "Platform/Process.h"

#include "GameSettings.h"
#include "Hash.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <ToshiTools/T2CommandLine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

#define UI_MAIN_PADDING 28

static struct MemoryInitialiser
{
	MemoryInitialiser()
	{
		TMemory::Initialise( 0, 0, TMemoryDL::Flags_NativeMethods );
	}
} g_oMemInitialiser;

Application g_oTheApp;

int main( int argc, char** argv )
{
	// Initialise engine
	TUtil::TOSHIParams engineParams;
	engineParams.szLogAppName  = "launcher";
	engineParams.szLogFileName = "Launcher";
	engineParams.szCommandLine = GetCommandLineA();

	TUtil::ToshiCreate( engineParams );
	g_oTheApp.Create( "BYardLauncher", argc, argv );
	g_oTheApp.Execute();

	//TUtil::ToshiDestroy();

	return 0;
}

TBOOL Application::OnEvent( const SDL_Event& event )
{
	ImGui_ImplSDL2_ProcessEvent( &event );

	if ( event.type == SDL_QUIT )
		g_oTheApp.Destroy();

	return TTRUE;
}

TBOOL Application::OnCreate( TINT argc, TCHAR** argv )
{
	TApplication::OnCreate( argc, argv );
	m_oWindowParams.pchTitle    = "Barnyard Launcher";
	m_oWindowParams.uiWidth     = 800;
	m_oWindowParams.uiHeight    = 600;
	m_oWindowParams.bIsWindowed = TTRUE;

	T2Render* pRender        = T2Render::CreateSingleton();
	TBOOL     bWindowCreated = pRender->Create( m_oWindowParams );
	TASSERT( TTRUE == bWindowCreated );

	T2Window* pWindow = pRender->GetWindow();
	pWindow->SetListener( this );

	// Initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeui.ttf", 20.0f, TNULL, io.Fonts->GetGlyphRangesDefault() );

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style  = ImGui::GetStyle();
	ImVec4*     colors = style.Colors;

	// Base color scheme
	colors[ ImGuiCol_Text ]                  = ImVec4( 0.92f, 0.92f, 0.92f, 1.00f );
	colors[ ImGuiCol_TextDisabled ]          = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
	colors[ ImGuiCol_WindowBg ]              = ImVec4( 0.13f, 0.14f, 0.15f, 1.00f );
	colors[ ImGuiCol_ChildBg ]               = ImVec4( 0.13f, 0.14f, 0.15f, 1.00f );
	colors[ ImGuiCol_PopupBg ]               = ImVec4( 0.10f, 0.10f, 0.11f, 0.94f );
	colors[ ImGuiCol_Border ]                = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
	colors[ ImGuiCol_BorderShadow ]          = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_FrameBg ]               = ImVec4( 0.20f, 0.21f, 0.22f, 1.00f );
	colors[ ImGuiCol_FrameBgHovered ]        = ImVec4( 0.25f, 0.26f, 0.27f, 1.00f );
	colors[ ImGuiCol_FrameBgActive ]         = ImVec4( 0.18f, 0.19f, 0.20f, 1.00f );
	colors[ ImGuiCol_TitleBg ]               = ImVec4( 0.15f, 0.15f, 0.16f, 1.00f );
	colors[ ImGuiCol_TitleBgActive ]         = ImVec4( 0.15f, 0.15f, 0.16f, 1.00f );
	colors[ ImGuiCol_TitleBgCollapsed ]      = ImVec4( 0.15f, 0.15f, 0.16f, 1.00f );
	colors[ ImGuiCol_MenuBarBg ]             = ImVec4( 0.20f, 0.20f, 0.21f, 1.00f );
	colors[ ImGuiCol_ScrollbarBg ]           = ImVec4( 0.20f, 0.21f, 0.22f, 1.00f );
	colors[ ImGuiCol_ScrollbarGrab ]         = ImVec4( 0.28f, 0.28f, 0.29f, 1.00f );
	colors[ ImGuiCol_ScrollbarGrabHovered ]  = ImVec4( 0.33f, 0.34f, 0.35f, 1.00f );
	colors[ ImGuiCol_ScrollbarGrabActive ]   = ImVec4( 0.40f, 0.40f, 0.41f, 1.00f );
	colors[ ImGuiCol_CheckMark ]             = ImVec4( 0.76f, 0.76f, 0.76f, 1.00f );
	colors[ ImGuiCol_SliderGrab ]            = ImVec4( 0.28f, 0.56f, 1.00f, 1.00f );
	colors[ ImGuiCol_SliderGrabActive ]      = ImVec4( 0.37f, 0.61f, 1.00f, 1.00f );
	colors[ ImGuiCol_Button ]                = ImVec4( 1.0f, 1.0f, 1.0f, 0.12f );
	colors[ ImGuiCol_ButtonHovered ]         = ImVec4( 1.0f, 1.0f, 1.0f, 0.16f );
	colors[ ImGuiCol_ButtonActive ]          = ImVec4( 0.25f, 0.30f, 0.35f, 1.00f );
	colors[ ImGuiCol_Header ]                = ImVec4( 0.25f, 0.25f, 0.25f, 0.80f );
	colors[ ImGuiCol_HeaderHovered ]         = ImVec4( 0.30f, 0.30f, 0.30f, 0.80f );
	colors[ ImGuiCol_HeaderActive ]          = ImVec4( 0.35f, 0.35f, 0.35f, 0.80f );
	colors[ ImGuiCol_Separator ]             = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
	colors[ ImGuiCol_SeparatorHovered ]      = ImVec4( 0.33f, 0.67f, 1.00f, 1.00f );
	colors[ ImGuiCol_SeparatorActive ]       = ImVec4( 0.33f, 0.67f, 1.00f, 1.00f );
	colors[ ImGuiCol_ResizeGrip ]            = ImVec4( 0.28f, 0.56f, 1.00f, 1.00f );
	colors[ ImGuiCol_ResizeGripHovered ]     = ImVec4( 0.37f, 0.61f, 1.00f, 1.00f );
	colors[ ImGuiCol_ResizeGripActive ]      = ImVec4( 0.37f, 0.61f, 1.00f, 1.00f );
	colors[ ImGuiCol_Tab ]                   = ImVec4( 0.15f, 0.18f, 0.22f, 1.00f );
	colors[ ImGuiCol_TabHovered ]            = ImVec4( 0.38f, 0.48f, 0.69f, 1.00f );
	colors[ ImGuiCol_TabActive ]             = ImVec4( 0.28f, 0.38f, 0.59f, 1.00f );
	colors[ ImGuiCol_TabUnfocused ]          = ImVec4( 0.15f, 0.18f, 0.22f, 1.00f );
	colors[ ImGuiCol_TabUnfocusedActive ]    = ImVec4( 0.15f, 0.18f, 0.22f, 1.00f );
	colors[ ImGuiCol_DockingPreview ]        = ImVec4( 0.28f, 0.56f, 1.00f, 1.00f );
	colors[ ImGuiCol_DockingEmptyBg ]        = ImVec4( 0.13f, 0.14f, 0.15f, 1.00f );
	colors[ ImGuiCol_PlotLines ]             = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
	colors[ ImGuiCol_PlotLinesHovered ]      = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
	colors[ ImGuiCol_PlotHistogram ]         = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ ImGuiCol_PlotHistogramHovered ]  = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
	colors[ ImGuiCol_TableHeaderBg ]         = ImVec4( 0.19f, 0.19f, 0.20f, 1.00f );
	colors[ ImGuiCol_TableBorderStrong ]     = ImVec4( 0.31f, 0.31f, 0.35f, 1.00f );
	colors[ ImGuiCol_TableBorderLight ]      = ImVec4( 0.23f, 0.23f, 0.25f, 1.00f );
	colors[ ImGuiCol_TableRowBg ]            = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ ImGuiCol_TableRowBgAlt ]         = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
	colors[ ImGuiCol_TextSelectedBg ]        = ImVec4( 0.28f, 0.56f, 1.00f, 0.35f );
	colors[ ImGuiCol_DragDropTarget ]        = ImVec4( 0.28f, 0.56f, 1.00f, 0.90f );
	colors[ ImGuiCol_NavHighlight ]          = ImVec4( 0.28f, 0.56f, 1.00f, 1.00f );
	colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
	colors[ ImGuiCol_NavWindowingDimBg ]     = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
	colors[ ImGuiCol_ModalWindowDimBg ]      = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );

	// Style adjustments
	style.WindowRounding    = 0.0f;
	style.FrameRounding     = 6.0f;
	style.ScrollbarRounding = 0;

	style.WindowTitleAlign = ImVec2( 0.50f, 0.50f );
	style.WindowPadding    = ImVec2( 0.0f, 0.0f );
	style.WindowBorderSize = 0.0f;
	style.FramePadding     = ImVec2( 5.0f, 5.0f );
	style.ItemSpacing      = ImVec2( 6.0f, 6.0f );
	style.ItemInnerSpacing = ImVec2( 6.0f, 6.0f );
	style.IndentSpacing    = 25.0f;

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL( pWindow->GetNativeWindow(), pRender->GetGLContext() );
	ImGui_ImplOpenGL3_Init( "#version 130" );

	// Check if there's a modloader EXE file in this directory
	if ( TFile* pGameExe = TFile::Create( ".\\BYardModLoader.exe", TFILEMODE_READ ) )
	{
		m_bHasGame = TTRUE;

		pGameExe->Destroy();
	}

	// Do other preparation things...
	LoadResources();

	ModManager::ScanForMods();
	g_oSettings.Load();

	return bWindowCreated;
}

void Application::LoadResources()
{
	// Load background image
	{
		int    width, height, numComponents;
		TBYTE* pImageData = stbi_load( "Launcher\\Assets\\background.png", &width, &height, &numComponents, 4 );

		m_BackgroundTexture.Create( TEXTURE_FORMAT_R8G8B8A8_UNORM, width, height, pImageData );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		stbi_image_free( pImageData );
	}
}

TBOOL Application::OnUpdate( TFLOAT flDeltaTime )
{
	T2Render* pRender = T2Render::GetSingleton();
	T2Window* pWindow = pRender->GetWindow();

	// Update window to get new events
	pRender->Update( flDeltaTime );

	if ( !m_bCreatedMainScreen )
	// Load the main screen
	{
		MainScreen* pMainScreen = new MainScreen();
		pMainScreen->EnableGameButtons( m_bHasGame );
		g_oUIControl.ShowScreen( pMainScreen );
		m_bCreatedMainScreen = TTRUE;
	}
	else
		g_oUIControl.Update( flDeltaTime );

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Create main dock space
	ImGuiViewport* imViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos( imViewport->Pos );
	ImGui::SetNextWindowSize( imViewport->Size );
	ImGui::SetNextWindowViewport( imViewport->ID );

	ImGuiStyle& style = ImGui::GetStyle();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( UI_MAIN_PADDING, UI_MAIN_PADDING ) );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
	ImGui::GetBackgroundDrawList()->AddImage( m_BackgroundTexture.GetHandle(), ImVec2( 0, 0 ), ImVec2( 800, 600 ) );

	ImGui::PushStyleColor( ImGuiCol_WindowBg, ImVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	ImGui::Begin( "DockSpace", TNULL, window_flags );
	{
		ImGui::PopStyleVar( 2 );

		ImGuiID dockspaceId = ImGui::GetID( "MainDockspace" );
		ImGui::DockSpace( dockspaceId, ImVec2( 0, 0 ), ImGuiDockNodeFlags_NoTabBar );

		/*if ( ImGui::BeginMainMenuBar() )
		{
			if ( ImGui::BeginMenu( "Visit GitHub" ) )
			{
				if ( ImGui::MenuItem( "OpenBarnyard Repository" ) )
					Platform_OpenInShell( "https://github.com/InfiniteC0re/OpenBarnyard" );

				if ( ImGui::MenuItem( "Launcher Repository" ) )
					Platform_OpenInShell( "https://github.com/InfiniteC0re/BarnyardLauncher" );

				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu( "Discord" ) )
			{
				if ( ImGui::MenuItem( "OpenTOSHI" ) )
					Platform_OpenInShell( "https://discord.gg/ZYngJwhrsn" );

				if ( ImGui::MenuItem( "Barnyard Speedrunning" ) )
					Platform_OpenInShell( "https://discord.gg/BrVpSPfgT8" );

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}*/

		ImGui::SetNextWindowDockID( dockspaceId );
		ImGui::Begin( "Main Window", TNULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
		{
			g_oUIControl.Render();

			ImGui::End();
		}

		ImGui::End();
	}

	ImGui::PopStyleColor(); // ImGuiCol_WindowBg

	// Render to the window
	pRender->BeginScene();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

	if ( ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		SDL_Window*   backup_current_window  = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent( backup_current_window, backup_current_context );
	}

	SDL_Window*   backup_current_window  = SDL_GL_GetCurrentWindow();
	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

	pRender->EndScene();

	return TTRUE;
}
