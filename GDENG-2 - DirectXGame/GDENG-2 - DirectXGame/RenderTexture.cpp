#include "RenderTexture.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"

RenderTexture::RenderTexture(UINT width, UINT height)
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
	textureDesc.Width = width / 2;
	textureDesc.Height = height / 2;
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

void RenderTexture::SetRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain)
{
	ID3D11DeviceContext* context = device_context->getContext();

	context->OMSetRenderTargets(1, &m_render_target_view, swap_chain->m_dsv);
}

void RenderTexture::ClearRenderTarget(const DeviceContextPtr& device_context, const SwapChainPtr& swap_chain, float r, float g, float b, float a)
{
	ID3D11DeviceContext* context = device_context->getContext();

	FLOAT clear_color[] = { r, g, b, a };
	context->ClearRenderTargetView(m_render_target_view, clear_color);
	context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}
