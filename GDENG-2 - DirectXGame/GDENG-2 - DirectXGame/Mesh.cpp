#include "Mesh.h"
#include "DeviceContext.h"
#include "InputSystem.h"
#include "SwapChain.h"
#include "Vertex.h"
#include "ConstantData.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path), AGameObject("")
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	// Converting string
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), NULL, 0, NULL, NULL);
	std::string inputfile(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), &inputfile[0], size_needed, NULL, NULL);
	//std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!err.empty()) throw std::exception("Mesh not created successfully");

	if (!res) throw std::exception("Mesh not created successfully");

	if (shapes.size() > 1) throw std::exception("Mesh not created successfully");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_verts; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

				//Normals
				tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
				tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
				tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

				VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
				list_vertices.push_back(vertex);

				list_indices.push_back((unsigned int)index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
		(UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

}

Mesh::~Mesh()
{
}

void Mesh::update(float deltaTime)
{
	m_delta_time = deltaTime;
}

static float rotation = 0.f;
void Mesh::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContextPtr deviceContext = graphEngine->getRenderSystem()->getImmediateDeviceContext();


	cc.m_time = m_ticks * m_speed;
	
	if (m_delta_pos > 1.0f)  m_delta_pos = 0.0f;
	else m_delta_pos += m_delta_time * 0.1f;

	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setIdentity(); translationMatrix.setTranslation(getLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setIdentity(); scaleMatrix.setScale(getLocalScale());
	Vector3D rotation = getLocalRotation();
	Matrix4x4 zMatrix; zMatrix.setIdentity(); zMatrix.setRotationZ(rotation.m_z);
	Matrix4x4 xMatrix; xMatrix.setIdentity(); xMatrix.setRotationX(rotation.m_x);
	Matrix4x4 yMatrix; yMatrix.setIdentity(); yMatrix.setRotationY(rotation.m_y);

	// multiply scale to rotation, then product to translation
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	yMatrix *= zMatrix;
	xMatrix *= yMatrix;
	rotMatrix *= xMatrix;
	scaleMatrix *= rotMatrix;
	allMatrix *= scaleMatrix;
	allMatrix *= translationMatrix;
	cc.m_world = allMatrix;

	m_cb->update(deviceContext, &cc);
	deviceContext->setConstantBuffer(vertexShader, m_cb);
	deviceContext->setConstantBuffer(pixelShader, m_cb);

	deviceContext->setIndexBuffer(m_index_buffer);
	deviceContext->setVertexBuffer(m_vertex_buffer);

	deviceContext->drawIndexedTriangleList(m_index_buffer->getSizeIndexList(), 0, 0);
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

void Mesh::onKeyDown(int key)
{
	//std::cout << "SHEESH" << std::endl;
}

void Mesh::onKeyUp(int key)
{
}

void Mesh::onMouseMove(const Point& mouse_pos)
{
}

void Mesh::onLeftMouseDown(const Point& mouse_pos)
{
}

void Mesh::onLeftMouseUp(const Point& mouse_pos)
{
}

void Mesh::onRightMouseDown(const Point& mouse_pos)
{
}

void Mesh::onRightMouseUp(const Point& mouse_pos)
{
}
