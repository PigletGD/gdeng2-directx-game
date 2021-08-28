#pragma once

#include "Prerequisites.h"

#include <d3d11.h>

class SamplerState
{
public:
	SamplerState(RenderSystem* system);
	~SamplerState();

private:
	ID3D11SamplerState* m_sampler_state;

	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};