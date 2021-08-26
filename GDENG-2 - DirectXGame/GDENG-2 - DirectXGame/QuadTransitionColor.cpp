#include "QuadTransitionColor.h"

#include "GraphicsEngine.h"
#include "DeviceContext.h"

#include "ConstantData.h"

QuadTransitionColor::QuadTransitionColor(lerp_vertex v1, lerp_vertex v2, lerp_vertex v3, lerp_vertex v4)
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

QuadTransitionColor::~QuadTransitionColor()
{
}

void QuadTransitionColor::createBuffers(void* shader_byte_code, UINT size_byte_shader)
{
	m_lvb = GraphicsEngine::get()->getRenderSystem()->createLerpVertexBuffer();
	m_lvb->load(vertex_list, sizeof(lerp_vertex), size_vertex_list, shader_byte_code, size_byte_shader, GraphicsEngine::get()->getRenderSystem());
	
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);
}

void QuadTransitionColor::draw()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_lvb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawTriangleStrip(m_lvb->getListSize(), 0);
}
