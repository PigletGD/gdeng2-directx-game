#include "VertexBuffer.h"
#include "RenderSystem.h"

#include <exception>
#include <iostream>

VertexBuffer::VertexBuffer()
{
	m_buffer = 0;
	m_layout = 0;
}

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}

void VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader, RenderSystem* system)
{
	//if (m_buffer != NULL) m_buffer->Release();
	//if (m_layout != NULL) m_layout->Release();

	m_system = system;

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	HRESULT res = m_system->getDevice()->CreateBuffer(&buff_desc, &init_data, &m_buffer);

	if (FAILED(res)) throw std::exception("Vertex Buffer not created successfully");

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT size_layout = ARRAYSIZE(layout);

	res = m_system->getDevice()->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout);

	if (FAILED(res)) { std::cout << "failure\n"; throw std::exception("Vertex Buffer not created successfully"); }
}

void VertexBuffer::release()
{
	if (this != NULL) {
		m_layout->Release();
		m_buffer->Release();
		delete this;
	}
}

ID3D11Buffer* VertexBuffer::getBuffer()
{
	return m_buffer;
}

ID3D11InputLayout* VertexBuffer::getInputLayout()
{
	return m_layout;
}

UINT VertexBuffer::getVertexSize()
{
	return m_size_vertex;
}

UINT VertexBuffer::getListSize()
{
	return m_size_list;
}
