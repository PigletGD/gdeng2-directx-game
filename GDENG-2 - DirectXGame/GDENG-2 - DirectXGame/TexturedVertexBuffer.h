#pragma once
#include <d3d11.h>

#include "VertexBuffer.h"
#include "Prerequisites.h"

class TexturedVertexBuffer : public VertexBuffer
{
public:
	TexturedVertexBuffer();
	~TexturedVertexBuffer();

	virtual void load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system);
};