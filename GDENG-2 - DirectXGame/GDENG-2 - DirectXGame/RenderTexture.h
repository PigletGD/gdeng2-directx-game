#pragma once

#include <d3d11.h>

#include "Prerequisites.h"

class RenderTexture
{
public:
	RenderTexture(UINT width, UINT height);
	~RenderTexture();

	void SetRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain);
	void ClearRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain, float r, float g, float b, float a);

private:
	ID3D11Texture2D* m_render_target_texture;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11ShaderResourceView* m_shader_resource_view;

	friend class ViewportScreen;
};