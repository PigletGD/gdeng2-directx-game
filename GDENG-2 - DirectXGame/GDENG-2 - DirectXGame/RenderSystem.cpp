#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "LerpVertexBuffer.h"
#include "TexturedVertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "AlphaBlendState.h"

#include <d3dcompiler.h>
#include <exception>

RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;) {
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL,
			feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device,
			&m_feature_level, &m_imm_context);

		if (SUCCEEDED(res)) break;

		++driver_type_index;
	}

	if (FAILED(res)) throw std::exception("RenderSystem not created successfully");

	m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context, this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	D3D11_RASTERIZER_DESC rast_desc;
	ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));
	rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	res = m_d3d_device->CreateRasterizerState(&rast_desc, &m_rs_solid);

	if (FAILED(res)) throw std::exception("RenderSystem not created successfully");

	ZeroMemory(&rast_desc, sizeof(D3D11_RASTERIZER_DESC));
	rast_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	rast_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	res = m_d3d_device->CreateRasterizerState(&rast_desc, &m_rs_wireframe);

	if (FAILED(res)) throw std::exception("RenderSystem not created successfully");
}

RenderSystem::~RenderSystem()
{
	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_d3d_device->Release();
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr sc = nullptr;
	try {
		sc = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch (...) {}
	return sc;
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBufferPtr RenderSystem::createVertexBuffer()
{
	VertexBufferPtr vb = nullptr;
	try {
		vb = std::make_shared<VertexBuffer>();
	}
	catch (...) {}
	return vb;
}

LerpVertexBufferPtr RenderSystem::createLerpVertexBuffer()
{
	LerpVertexBufferPtr vb = nullptr;
	try {
		vb = std::make_shared<LerpVertexBuffer>();
	}
	catch (...) {}
	return vb;
}

TexturedVertexBufferPtr RenderSystem::createTexturedVertexBuffer()
{
	TexturedVertexBufferPtr vb = nullptr;
	try {
		vb = std::make_shared<TexturedVertexBuffer>();
	}
	catch (...) {}
	return vb;
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr ib = nullptr;
	try {
		ib = std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}
	catch (...) {}
	return ib;
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr cb = nullptr;
	try {
		cb = std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}
	catch (...) {}
	return cb;
}

AlphaBlendStatePtr RenderSystem::createAlphaBlendState()
{
	AlphaBlendStatePtr abs = nullptr;
	try {
		abs = std::make_shared<AlphaBlendState>(this);
	}
	catch (...) {}
	return abs;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShaderPtr vs = nullptr;
	try {
		vs = std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
	}
	catch (...) {}
	return vs;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	HRESULT res;
	res = ::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob);

	if (FAILED(res))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

PixelShaderPtr RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShaderPtr ps = nullptr;
	try {
		ps = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}
	catch (...) {}
	return ps;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	HRESULT res;
	res = ::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob);

	if (FAILED(res))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();
}

ID3D11Device* RenderSystem::getDevice()
{
	return m_d3d_device;
}