#include "AlphaBlendState.h"
#include "RenderSystem.h"
#include "DeviceContext.h"

#include <exception>

AlphaBlendState::AlphaBlendState(RenderSystem* system) :
	m_system(system)
{
	ID3D11BlendState* d3dBlendState;
	D3D11_BLEND_DESC blend_desc = {};
	//ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));

	auto& rend_target_desc = blend_desc.RenderTarget[0];
	rend_target_desc.BlendEnable = true;
	rend_target_desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rend_target_desc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rend_target_desc.BlendOp = D3D11_BLEND_OP_ADD;
	rend_target_desc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rend_target_desc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rend_target_desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rend_target_desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT res;
	res = m_system->m_d3d_device->CreateBlendState(&blend_desc, &m_blend_state);

	if (FAILED(res)) throw std::exception("AlphaBlendState not created successfully");
}

AlphaBlendState::~AlphaBlendState()
{
	m_blend_state->Release();
}
