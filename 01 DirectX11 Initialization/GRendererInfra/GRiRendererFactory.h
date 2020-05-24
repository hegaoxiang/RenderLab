#pragma once
#include "GRiTextureLoader.h"
class GRiRendererFactory
{
public:
	virtual GRiTextureLoader* CreateTextureLoader() = 0;
};

