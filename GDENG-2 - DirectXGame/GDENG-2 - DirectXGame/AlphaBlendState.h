#pragma once

#include "Prerequisites.h"

#include <d3d11.h>

class AlphaBlendState
{
public:
	AlphaBlendState(RenderSystem* system);
	~AlphaBlendState();

private:
	ID3D11BlendState* m_blend_state;

	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};