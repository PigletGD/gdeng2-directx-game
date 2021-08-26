#pragma once

#include <d3d11.h>

#include "Prerequisites.h"

class RenderTexture
{
public:
	RenderTexture(UINT width, UINT height);
	~RenderTexture();

	void clearRenderTarget(const DeviceContextPtr& device_context, float r, float g, float b, float a);
	void setViewportSize(const DeviceContextPtr& device_context, UINT width, UINT height);

	void resize(UINT width, UINT height);
private:
	void reloadBuffers(UINT width, UINT height);

	ID3D11Texture2D* m_rtt;
	ID3D11RenderTargetView* m_rtv;
	ID3D11ShaderResourceView* m_srv;

	ID3D11DepthStencilView* m_dsv;

	friend class ViewportScreen;
};

