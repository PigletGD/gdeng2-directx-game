#pragma once
#include "Prerequisites.h"
#include <d3d11.h>

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);
	~DeviceContext();

	void clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha);

	void setRasterizerState(ID3D11RasterizerState* rasterizer_state);

	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setVertexBuffer(const VertexColorBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count,  UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);

	void setAlphaBlendState(const AlphaBlendStatePtr& alpha_blend_state);

	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr& texture);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr& texture);

	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer);
	void setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer);

private:
	ID3D11DeviceContext* m_device_context;

	RenderSystem* m_system = nullptr;

	friend class ConstantBuffer;
	friend class SwapChain;
};