#pragma once
#include "IUIScreen.h"

class ModsScreen
    : public IUIScreen
{
public:
	ModsScreen();
	~ModsScreen();

	void Render() override;
	void Update( TFLOAT flDeltaTime ) override;
};
