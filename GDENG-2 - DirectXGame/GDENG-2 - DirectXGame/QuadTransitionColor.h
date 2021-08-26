#pragma once

#include "LerpVertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

#include "Prerequisites.h"

class QuadTransitionColor
{
public:
	QuadTransitionColor(lerp_vertex v1, lerp_vertex v2, lerp_vertex v3, lerp_vertex v4);
	~QuadTransitionColor();

	void createBuffers(void* shader_byte_code, UINT size_byte_shader);

	void draw();
private:
	LerpVertexBufferPtr m_lvb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	lerp_vertex vertex_list[4];
	UINT size_vertex_list;

	unsigned int index_list[6];
	UINT size_index_list;
};
