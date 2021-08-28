#pragma once
#include <d3d11.h>

#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr getImmediateDeviceContext();

	VertexBufferPtr createVertexBuffer();
	LerpVertexBufferPtr createLerpVertexBuffer();
	TexturedVertexBufferPtr createTexturedVertexBuffer();

	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
	AlphaBlendStatePtr createAlphaBlendState();
	SamplerStatePtr createSamplerState();

	VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	void releaseCompiledShader();

	ID3D11Device* getDevice();

private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;


	// Private members of Rendering System Class
	ID3D11RasterizerState* m_rs_solid;
	ID3D11RasterizerState* m_rs_wireframe;

	DeviceContextPtr m_imm_device_context;

	ID3DBlob* m_blob = nullptr;

	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;

	friend class AppWindow;
	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexColorBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class AlphaBlendState;
	friend class SamplerState;
	friend class Texture;
	friend class ViewportScreen;
	friend class MenuBarScreen;
};