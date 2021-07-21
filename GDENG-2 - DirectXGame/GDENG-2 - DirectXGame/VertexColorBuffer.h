#pragma once
#include <d3d11.h>

#include "Prerequisites.h"

class VertexColorBuffer
{
public:
	VertexColorBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system);
	~VertexColorBuffer();

	UINT getSizeVertexList();

private:
	UINT m_size_vertex;
	UINT m_size_list;

	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;

	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};

