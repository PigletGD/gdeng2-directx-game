#pragma once
#include <d3d11.h>

#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

	void resize(unsigned int width, unsigned int height);
	bool present(bool vsync);

private:

	void reloadBuffers(unsigned int width, unsigned int height);

	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;

	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
	friend class RenderTexture;
	friend class ViewportScreen;
};