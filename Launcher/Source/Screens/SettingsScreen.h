#pragma once
#include "IUIScreen.h"

#include <ToshiTools/T2DynamicVector.h>
#include <Toshi/TString8.h>

class SettingsScreen
    : public IUIScreen
{
public:
	SettingsScreen();
	~SettingsScreen();

	void Render() override;
	void Update( TFLOAT flDeltaTime ) override;

private:
	Toshi::T2DynamicVector<Toshi::TString8> m_vecResolutions;
	TINT                                    m_iSelectedResolution = 0;
};
