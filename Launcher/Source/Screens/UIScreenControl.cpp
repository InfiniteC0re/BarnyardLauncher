#include "pch.h"
#include "UIScreenControl.h"

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

UIScreenControl g_oUIControl;

UIScreenControl::UIScreenControl()
{
}

UIScreenControl::~UIScreenControl()
{
}

void UIScreenControl::Update( TFLOAT flDeltaTime )
{
	if ( m_pCurrScreen )
		m_pCurrScreen->Update( flDeltaTime );

	if ( m_bAnimatedClose && !m_pCurrScreen->HasAnyAnimation() )
	// Asked to close and the animation is over
	{
		delete m_pCurrScreen;
		m_pCurrScreen = m_pNextScreen;
		m_pCurrScreen->StartAnimFadeIn( m_flNextScreenFadeInSpeed );
		m_pNextScreen    = TNULL;
		m_bAnimatedClose = TFALSE;
	}
}

void UIScreenControl::Render()
{
	if ( m_pCurrScreen )
	{
		m_pCurrScreen->Render();
	}
}

void UIScreenControl::ShowScreen( IUIScreen* pScreen, TFLOAT flOutSpeed, TFLOAT flInSpeed )
{
	if ( m_pCurrScreen )
	{
		TBOOL bAnimatedClose = CloseScreen( flOutSpeed );

		if ( bAnimatedClose )
		{
			m_pNextScreen             = pScreen;
			m_flNextScreenFadeInSpeed = flInSpeed;
			return;
		}
	}

	m_pCurrScreen = pScreen;
	m_pCurrScreen->StartAnimFadeIn( flInSpeed );
}

TBOOL UIScreenControl::CloseScreen( TFLOAT flOutSpeed )
{
	if ( m_pCurrScreen )
	{
		m_bAnimatedClose = ( flOutSpeed > 0.0f );
	
		if ( m_bAnimatedClose )
		{
			// Close is animated
			m_pCurrScreen->StartAnimFadeOut( flOutSpeed );
		}
		else
		{
			// No animation, so just destroy the screen
			delete m_pCurrScreen;
			m_pCurrScreen = TNULL;
		}
	}
	else
	{
		// No screen is currently visible
		m_bAnimatedClose = TFALSE;
	}

	return m_bAnimatedClose;
}
