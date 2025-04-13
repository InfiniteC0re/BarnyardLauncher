#include "pch.h"
#include "IUIScreen.h"

//-----------------------------------------------------------------------------
// Enables memory debugging.
// Note: Should be the last include!
//-----------------------------------------------------------------------------
#include <Core/TMemoryDebugOn.h>

TOSHI_NAMESPACE_USING

IUIScreen::IUIScreen()
{
}

IUIScreen::~IUIScreen()
{
	ImGui::GetStyle().Alpha = 1.0f;
}

void IUIScreen::Render()
{
}

void IUIScreen::Update( TFLOAT flDeltaTime )
{
	if ( m_bAnimInProgress )
	// Update animation progress
	{
		m_flAnimCurrentTime += flDeltaTime;

		if ( m_flAnimCurrentTime >= m_flAnimDuration )
		{
			m_flAnimCurrentTime = m_flAnimDuration;
			m_bAnimInProgress   = TFALSE;
		}
	}
}

void IUIScreen::StartAnimFadeIn( TFLOAT flDuration )
{
	m_flAnimCurrentTime  = 0.0f;
	m_flAnimDuration     = flDuration;
	m_bAnimInProgress    = TTRUE;
	m_bAnimIsFadeIn      = TTRUE;
}

void IUIScreen::StartAnimFadeOut( TFLOAT flDuration )
{
	m_flAnimCurrentTime = 0.0f;
	m_flAnimDuration    = flDuration;
	m_bAnimInProgress   = TTRUE;
	m_bAnimIsFadeIn     = TFALSE;
}

TFLOAT IUIScreen::GetAnimProgress() const
{
	TBOOL bIsFadingIn = IsFadingIn();

	if ( m_bAnimInProgress )
	{
		TFLOAT flProgress = EaseOutCubic( m_flAnimCurrentTime / m_flAnimDuration );

		return ( bIsFadingIn ) ? flProgress : 1.0f - flProgress;
	}
	
	return bIsFadingIn ? 1.0f : 1.0f;
}

TBOOL IUIScreen::IsFadingIn() const
{
	return m_bAnimIsFadeIn;
}

TBOOL IUIScreen::HasAnyAnimation() const
{
	return m_bAnimInProgress;
}

TFLOAT IUIScreen::EaseInCubic( TFLOAT flProgress )
{
	return flProgress * flProgress * flProgress;
}

TFLOAT IUIScreen::EaseOutCubic( TFLOAT flProgress )
{
	return 1.0f - powf( 1.0f - flProgress, 3.0f );
}

TFLOAT IUIScreen::EaseInOutCubic( TFLOAT flProgress )
{
	return flProgress < 0.5f ? 4.0f * flProgress * flProgress * flProgress : 1.0f - powf( -2.0f * flProgress + 2.0f, 3.0f ) / 2.0f;
}
