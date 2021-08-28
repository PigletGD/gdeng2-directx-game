#include "SamplerState.h"
#include "RenderSystem.h"
#include "DeviceContext.h"

#include <exception>
#include <iostream>

SamplerState::SamplerState(RenderSystem* system) :
	m_system(system)
{
	D3D11_SAMPLER_DESC sampler_desc;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT res = m_system->m_d3d_device->CreateSamplerState(&sampler_desc, &m_sampler_state);

	if (FAILED(res)) throw std::exception("Sampler State not created successfully");
}

SamplerState::~SamplerState()
{
	m_sampler_state->Release();
}
