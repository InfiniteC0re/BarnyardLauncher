#pragma once
#include "IUIScreen.h"

class UIScreenControl
{
public:
	UIScreenControl();
	~UIScreenControl();

	void Update( TFLOAT flDeltaTime );
	void Render();
	void ShowScreen( IUIScreen* pScreen, TFLOAT flOutSpeed = 0.3f, TFLOAT flInSpeed = 0.3f );

	// Returns TTRUE if animation started
	TBOOL CloseScreen( TFLOAT flOutSpeed = 0.3f );

	IUIScreen* GetScreen() const { return m_pCurrScreen; }
	IUIScreen* GetNextScreen() const { return m_pNextScreen; }

private:
	IUIScreen* m_pCurrScreen             = TNULL;
	IUIScreen* m_pNextScreen             = TNULL;
	TFLOAT     m_flNextScreenFadeInSpeed = 0.0f;
	TBOOL      m_bAnimatedClose          = TFALSE;
};

extern UIScreenControl g_oUIControl;
