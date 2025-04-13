#pragma once
#include "IUIScreen.h"

class MainScreen
    : public IUIScreen
{
public:
	MainScreen();
	~MainScreen();

	void Render() override;
	void Update( TFLOAT flDeltaTime ) override;

	void EnableGameButtons( TBOOL bEnable );

private:
	TBOOL m_bEnableGameButtons = TFALSE;
};
