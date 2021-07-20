#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

#include "Prerequisites.h"

class Quad
{
public:
	Quad(vertex v1, vertex v2, vertex v3, vertex v4);
	~Quad();

	void createBuffers(void* shader_byte_code, UINT size_byte_shader);

	void draw();
	void update(Vector3D pos1, Vector3D pos2, Vector3D pos3, Vector3D pos4);
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	vertex vertex_list[4];
	UINT size_vertex_list;

	unsigned int index_list[6];
	UINT size_index_list;
};