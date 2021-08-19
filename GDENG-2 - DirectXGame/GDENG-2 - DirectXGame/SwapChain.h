#pragma once
#include <d3d11.h>

#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

	bool present(bool vsync);

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;
	ID3D11RasterizerState* m_rs;

	ID3D11Texture2D* m_rttm;
	ID3D11RenderTargetView* m_rtv2;
	ID3D11ShaderResourceView* m_srv;

	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
	friend class CreditsScreen;
};