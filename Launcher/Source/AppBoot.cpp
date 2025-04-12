#include "pch.h"

#include "Platform/Utils.h"
#include "Platform/Process.h"

#include "GameSettings.h"
#include "Hash.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <Toshi/Toshi.h>
#include <Toshi/TApplication.h>
#include <Render/T2Render.h>
#include <ToshiTools/T2CommandLine.h>

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

static struct MemoryInitialiser
{
	MemoryInitialiser()
	{
		TMemory::Initialise( 0, 0, TMemoryDL::Flags_NativeMethods );
	}
} g_oMemInitialiser;

class Application
    : public TApplication
    , public T2Window::EventListener
{
public:
	//-----------------------------------------------------------------------------
	// T2Window::EventListener
	//-----------------------------------------------------------------------------
	virtual TBOOL OnEvent( const SDL_Event& event ) OVERRIDE
	{
		ImGui_ImplSDL2_ProcessEvent( &event );

		if ( event.type == SDL_QUIT )
			g_oTheApp.Destroy();

		return TTRUE;
	}

	//-----------------------------------------------------------------------------
	// Toshi::TApplication
	//-----------------------------------------------------------------------------
	virtual TBOOL OnCreate( TINT argc, TCHAR** argv ) OVERRIDE
	{
		TApplication::OnCreate( argc, argv );

		T2Render::WindowParams windowParams;
		windowParams.pchTitle    = "Barnyard Launcher";
		windowParams.uiWidth     = 500;
		windowParams.uiHeight    = 260;
		windowParams.bIsWindowed = TTRUE;

		T2Render* pRender        = T2Render::CreateSingleton();
		TBOOL     bWindowCreated = pRender->Create( windowParams );
		TASSERT( TTRUE == bWindowCreated );

		T2Window* pWindow = pRender->GetWindow();
		pWindow->SetListener( this );

		// Initialise ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.Fonts->AddFontFromFileTTF( "Bahnschrift.ttf", 17.0f, TNULL, io.Fonts->GetGlyphRangesDefault() );

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
		colors[ ImGuiCol_Button ]                = ImVec4( 0.20f, 0.25f, 0.30f, 1.00f );
		colors[ ImGuiCol_ButtonHovered ]         = ImVec4( 0.30f, 0.35f, 0.40f, 1.00f );
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
		style.WindowRounding    = 5.3f;
		style.FrameRounding     = 2.3f;
		style.ScrollbarRounding = 0;

		style.WindowTitleAlign = ImVec2( 0.50f, 0.50f );
		style.WindowPadding    = ImVec2( 8.0f, 8.0f );
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
		ObtainAllScreenResolutions();

		return bWindowCreated;
	}

	void ObtainAllScreenResolutions()
	{
		TINT iDisplayIndex    = 0;
		TINT iNumDisplayModes = SDL_GetNumDisplayModes( iDisplayIndex );

		T2Map<TUINT32, TBOOL> mapResolutions;
		for ( TINT i = 0; i < iNumDisplayModes; i++ )
		{
			SDL_DisplayMode displayMode;
			SDL_GetDisplayMode( iDisplayIndex, i, &displayMode );

			TUINT32 uiHash = Hash_Vec2i( displayMode.w, displayMode.h );
			if ( mapResolutions.Find( uiHash ) == mapResolutions.End() )
			{
				TINFO( "Found new screen resolution (%dx%d)\n", displayMode.w, displayMode.h );

				m_vecResolutions.PushBack( std::move( TString8::VarArgs( "%dx%d", displayMode.w, displayMode.h ) ) );
				mapResolutions.Insert( uiHash, TTRUE );
			}
		}
	}

	virtual TBOOL OnUpdate( TFLOAT flDeltaTime ) OVERRIDE
	{
		T2Render* pRender = T2Render::GetSingleton();
		T2Window* pWindow = pRender->GetWindow();

		// Update window to get new events
		pRender->Update( flDeltaTime );

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Create main dock space
		ImGuiViewport* imViewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos( imViewport->Pos );
		ImGui::SetNextWindowSize( imViewport->Size );
		ImGui::SetNextWindowViewport( imViewport->ID );

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
		ImGui::Begin( "DockSpace", TNULL, window_flags );
		{
			ImGui::PopStyleVar();

			ImGuiID dockspaceId = ImGui::GetID( "MainDockspace" );
			ImGui::DockSpace( dockspaceId, ImVec2( 0, 0 ), ImGuiDockNodeFlags_NoTabBar );

			if ( ImGui::BeginMainMenuBar() )
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
			}

			ImGui::SetNextWindowDockID( dockspaceId );
			ImGui::Begin( "Main Window", TNULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );
			{
				if ( !m_bHasGame )
				{
					ImGui::PushStyleColor( ImGuiCol_Text, ImVec4( 1.0f, 0.0f, 0.0f, 1.00f ) );
					ImGui::Text( "Error: No game found! Make sure the launcher is in the correct directory!" );
					ImGui::PopStyleColor();
				}

				ImGui::Text( "Game Settings" );

				ImGui::BeginDisabled( !m_bHasGame );
				{
					if ( ImGui::BeginCombo( "Resolution", m_vecResolutions[ m_iSelectedResolution ] ) )
					{
						T2_FOREACH( m_vecResolutions, it )
						{
							TBOOL bSelected = ( it.Index() == m_iSelectedResolution );

							if ( ImGui::Selectable( it.Get()->GetString(), &bSelected ) )
								m_iSelectedResolution = it.Index();

							if ( bSelected )
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					ImGui::Checkbox( "Windowed Mode", &g_oSettings.bWindowed );
					ImGui::Separator();

					ImGui::Text( "Modloader" );
					ImGui::Checkbox( "Experimental Mode", &g_oSettings.bExperimental );
					ImGui::Checkbox( "Fun%", &g_oSettings.bFun );

					if ( ImGui::Button( "Play Game" ) )
					{
						// Create a string with all launch parameters
						T2FormatWString512 strStartParams;
						{
							// Resolution
							strStartParams.Append( g_oSettings.bWindowed ? L"-windowed " : L"-fullscreen " );

							const TString8& strResolution = m_vecResolutions[ m_iSelectedResolution ];
							TINT iDivider = strResolution.Find( 'x' );

							if ( iDivider != -1 )
							{
								TString8 strWidth  = strResolution.Mid( 0, iDivider );
								TString8 strHeight = strResolution.Right( iDivider + 1 );

								TINT iWidth  = T2String8::StringToInt( strWidth );
								TINT iHeight = T2String8::StringToInt( strHeight );

								T2FormatWString128 resolutionParams;
								resolutionParams.Format( L"-width %d -height %d ", iWidth, iHeight );

								strStartParams.Append( resolutionParams.Get() );
							}

							// Other parameters
							if ( g_oSettings.bExperimental )
								strStartParams.Append( L"-experimental " );

							if ( g_oSettings.bFun )
								strStartParams.Append( L"-fun " );
						}

						Process gameProcess;
						gameProcess.Create(
						    L".\\BYardModLoader.exe",
						    strStartParams.Get(),
						    L".\\"
						);
					}

					ImGui::EndDisabled();
				}
				
				ImGui::End();
			}

			ImGui::End();
		}

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

private:
	TBOOL                     m_bHasGame = TFALSE;
	T2DynamicVector<TString8> m_vecResolutions;
	TINT                      m_iSelectedResolution;

} g_oTheApp;

int main( int argc, char** argv )
{
	// Initialise engine
	TUtil::TOSHIParams engineParams;
	engineParams.szLogAppName  = "TRV";
	engineParams.szLogFileName = "tresourceviewer";
	engineParams.szCommandLine = GetCommandLineA();

	TUtil::ToshiCreate( engineParams );
	g_oTheApp.Create( "Toshi Resource Viewer", argc, argv );
	g_oTheApp.Execute();

	//TUtil::ToshiDestroy();

	return 0;
}
