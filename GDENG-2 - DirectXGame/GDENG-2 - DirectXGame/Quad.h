#pragma once

#include "TexturedVertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

#include "Prerequisites.h"

class Quad
{
public:
	Quad(textured_vertex v1, textured_vertex v2, textured_vertex v3, textured_vertex v4);
	~Quad();

	void createBuffers(void* shader_byte_code, UINT size_byte_shader);

	void draw();
private:
	TexturedVertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	textured_vertex vertex_list[4];
	UINT size_vertex_list;

	unsigned int index_list[6];
	UINT size_index_list;
};