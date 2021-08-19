#pragma once

#include <d3d11.h>

#include "Prerequisites.h"

class RenderTexture
{
public:
	RenderTexture(UINT width, UINT height);
	~RenderTexture();

	void setRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain);
	void clearRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain, float r, float g, float b, float a);
	void setViewportSize(const DeviceContextPtr& device_context, UINT width, UINT height);

	void resize(UINT width, UINT height);
private:
	void reloadBuffers(UINT width, UINT height);

	ID3D11Texture2D* m_render_target_texture;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11ShaderResourceView* m_shader_resource_view;

	ID3D11DepthStencilView* m_dsv;

	friend class ViewportScreen;
};