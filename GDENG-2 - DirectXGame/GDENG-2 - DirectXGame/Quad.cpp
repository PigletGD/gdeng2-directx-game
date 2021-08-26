#include "Quad.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include "ConstantData.h"

Quad::Quad(textured_vertex v1, textured_vertex v2, textured_vertex v3, textured_vertex v4)
{
	vertex_list[0] = v1;
	vertex_list[1] = v2;
	vertex_list[2] = v3;
	vertex_list[3] = v4;
	size_vertex_list = ARRAYSIZE(vertex_list);

	index_list[0] = 0;
	index_list[1] = 1;
	index_list[2] = 3;
	index_list[3] = 1;
	index_list[4] = 2;
	index_list[5] = 3;
	size_index_list = ARRAYSIZE(index_list);
}

Quad::~Quad()
{
}

void Quad::createBuffers(void* shader_byte_code, UINT size_shader)
{
	m_vb = GraphicsEngine::get()->getRenderSystem()->createTexturedVertexBuffer();
	m_vb->load(vertex_list, sizeof(textured_vertex), size_vertex_list, shader_byte_code, size_shader, GraphicsEngine::get()->getRenderSystem());

	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);
}

void Quad::draw()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
