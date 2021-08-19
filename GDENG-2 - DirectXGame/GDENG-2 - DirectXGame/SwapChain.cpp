#include "SwapChain.h"
#include "RenderSystem.h"

#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system) :
	m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	//desc.BufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

	// Gets back buffer
	ID3D11Texture2D* buffer;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	renderTargetViewDesc.Format = desc.BufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(buffer, &renderTargetViewDesc, &m_rtv);

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

	shaderResourceViewDesc.Format = desc.BufferDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(buffer, &shaderResourceViewDesc, &m_srv);
	buffer->Release();

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

	// Depth Buffer

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

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);
	buffer->Release();

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");

	/*mod

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
	device->CreateTexture2D(&textureDesc, NULL, &m_rttm);

	/////////////////////// Map's Render Target
// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	device->CreateRenderTargetView(m_rttm, &renderTargetViewDesc, &m_rtv2);

	/////////////////////// Map's Shader Resource View
// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	device->CreateShaderResourceView(m_rttm, &shaderResourceViewDesc, &m_srv);

	/*mod*/

	D3D11_RASTERIZER_DESC rast_desc;
	ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));
	rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

	hr = device->CreateRasterizerState(&rast_desc, &m_rs);

	if (FAILED(hr)) throw std::exception("SwapChain not created successfully");
}

SwapChain::~SwapChain()
{
	m_rs->Release();
	m_rtv->Release();
	m_dsv->Release();
	m_swap_chain->Release();
}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);

	return true;
}
