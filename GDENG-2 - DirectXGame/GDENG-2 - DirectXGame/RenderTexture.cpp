#include "RenderTexture.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"

RenderTexture::RenderTexture(UINT width, UINT height)
{
	// for some reason, don't initialize buffers on start?
}

RenderTexture::~RenderTexture()
{
	if (m_srv) {
		m_srv->Release();
		m_srv = 0;
	}
	if (m_rtv) {
		m_rtv->Release();
		m_rtv = 0;
	}
	if (m_rtt) {
		m_rtt->Release();
		m_rtt = 0;
	}
}

void RenderTexture::setViewportSize(const DeviceContextPtr& device_context, UINT width, UINT height)
{
	ID3D11DeviceContext* context = device_context->getContext();

	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->RSSetViewports(1, &vp);
}

void RenderTexture::resize(UINT width, UINT height)
{
	if (m_dsv) {
		m_dsv->Release();
		m_dsv = 0;
	}
	if (m_srv) {
		m_srv->Release();
		m_srv = 0;
	}
	if (m_rtv) {
		m_rtv->Release();
		m_rtv = 0;
	}
	if (m_rtt) {
		m_rtt->Release();
		m_rtt = 0;
	}

	reloadBuffers(width, height);
}

void RenderTexture::reloadBuffers(UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->getRenderSystem()->getDevice();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};

	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, NULL, &m_rtt);

	if (FAILED(hr)) throw std::exception("Render Texture not created successfully");

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(m_rtt, &renderTargetViewDesc, &m_rtv);

	if (FAILED(hr)) throw std::exception("Render Texture not created successfully");

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(m_rtt, &shaderResourceViewDesc, &m_srv);

	if (FAILED(hr)) throw std::exception("Render Texture not created successfully");

	ID3D11Texture2D* buffer;
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	hr = device->CreateTexture2D(&tex_desc, nullptr, &buffer);

	if (FAILED(hr)) throw std::exception("Render Texture not created successfully");

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);

	if (FAILED(hr)) throw std::exception("Render Texture not created successfully");
}

void RenderTexture::clearRenderTarget(const DeviceContextPtr& device_context, float r, float g, float b, float a)
{
	ID3D11DeviceContext* context = device_context->getContext();

	FLOAT clear_color[] = { r, g, b, a };
	context->ClearRenderTargetView(m_rtv, clear_color);
	context->ClearDepthStencilView(m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	context->OMSetRenderTargets(1, &m_rtv, m_dsv);

}