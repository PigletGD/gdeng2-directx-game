#include "RenderTexture.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"

RenderTexture::RenderTexture(UINT width, UINT height)
{
	reloadBuffers(width, height);
}

RenderTexture::~RenderTexture()
{
	if (m_shader_resource_view) {
		m_shader_resource_view->Release();
		m_shader_resource_view = 0;
	}
	if (m_render_target_view) {
		m_render_target_view->Release();
		m_render_target_texture = 0;
	}
	if (m_render_target_texture) {
		m_render_target_texture->Release();
		m_render_target_texture = 0;
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
	if (m_shader_resource_view) {
		m_shader_resource_view->Release();
		m_shader_resource_view = 0;
	}
	if (m_render_target_view) {
		m_render_target_view->Release();
		m_render_target_texture = 0;
	}
	if (m_render_target_texture) {
		m_render_target_texture->Release();
		m_render_target_texture = 0;
	}

	reloadBuffers(width, height);
}

void RenderTexture::reloadBuffers(UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::get()->getRenderSystem()->getDevice();

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a render target AND a shader resource
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

	// Create the texture
	device->CreateTexture2D(&textureDesc, NULL, &m_render_target_texture);

	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	device->CreateRenderTargetView(m_render_target_texture, &renderTargetViewDesc, &m_render_target_view);

	/////////////////////// Map's Shader Resource View
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	device->CreateShaderResourceView(m_render_target_texture, &shaderResourceViewDesc, &m_shader_resource_view);

	// Depth Buffer

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

	device->CreateTexture2D(&tex_desc, nullptr, &buffer);

	device->CreateDepthStencilView(buffer, NULL, &m_dsv);
}

void RenderTexture::setRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain)
{
	ID3D11DeviceContext* context = device_context->getContext();

	context->OMSetRenderTargets(1, &m_render_target_view, m_dsv);
}

void RenderTexture::clearRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain, float r, float g, float b, float a)
{
	ID3D11DeviceContext* context = device_context->getContext();

	FLOAT clear_color[] = { r, g, b, a };
	context->ClearRenderTargetView(m_render_target_view, clear_color);
	context->ClearDepthStencilView(m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}
