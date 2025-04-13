#pragma once
#include <imgui.h>

class IUIScreen
{
public:
	IUIScreen();
	virtual ~IUIScreen();

	virtual void Render();
	virtual void Update( TFLOAT flDeltaTime );
	virtual void StartAnimFadeIn( TFLOAT flDuration );
	virtual void StartAnimFadeOut( TFLOAT flDuration );

	TFLOAT GetAnimProgress() const;
	TBOOL  IsFadingIn() const;
	TBOOL  HasAnyAnimation() const;

	static TFLOAT EaseInCubic( TFLOAT flProgress );
	static TFLOAT EaseOutCubic( TFLOAT flProgress );
	static TFLOAT EaseInOutCubic( TFLOAT flProgress );

protected:
	TFLOAT m_flAnimCurrentTime = 0.0f;
	TFLOAT m_flAnimDuration    = 0.0f;
	TBOOL  m_bAnimInProgress   = TFALSE;
	TBOOL  m_bAnimIsFadeIn     = TTRUE;
};
