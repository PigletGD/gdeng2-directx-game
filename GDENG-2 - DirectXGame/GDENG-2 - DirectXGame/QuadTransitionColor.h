#pragma once

#include "VertexColorBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

#include "Prerequisites.h"

class QuadTransitionColor
{
public:
	QuadTransitionColor(vertex_color v1, vertex_color v2, vertex_color v3, vertex_color v4);
	~QuadTransitionColor();

	void createBuffers(void* shader_byte_code, UINT size_byte_shader);

	void draw();
private:
	VertexColorBufferPtr m_vcb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	vertex_color vertex_list[4];
	UINT size_vertex_list;

	unsigned int index_list[6];
	UINT size_index_list;
};
