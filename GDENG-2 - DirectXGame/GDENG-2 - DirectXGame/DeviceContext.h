#pragma once
#include <d3d11.h>

#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	~DeviceContext();

	void setRenderTarget(const SwapChainPtr& swap_chain);

	void clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha);

	void setRasterizerState(ID3D11RasterizerState* rs);
	
	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count,  UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);

	void setAlphaBlendState(const AlphaBlendStatePtr& alpha_blend_state);

	void setTexture(const TexturePtr& texture);

	void setRenderConfig(const VertexShaderPtr& vertex_shader, const PixelShaderPtr& pixel_shader);

	void setConstantBuffer(const ConstantBufferPtr& buffer);

	ID3D11DeviceContext* getContext();

private:
	ID3D11DeviceContext* m_device_context;

	RenderSystem* m_system = nullptr;

	friend class AppWindow;
	friend class ConstantBuffer;
	friend class SwapChain;
};