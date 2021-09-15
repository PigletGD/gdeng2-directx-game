#include "Cylinder.h"
#include "GraphicsEngine.h"
#include "ShaderLibrary.h"
#include "Vertex.h"
#include <Windows.h>

#include "ConstantBuffer.h"
#include "DeviceContext.h"

Cylinder::Cylinder(std::string name, AGameObject::PrimitiveType type) : AGameObject(name, type)
{
	ShaderNames shader_names;
	void* shader_byte_code = NULL;
	size_t size_shader = 0;
	ShaderLibrary::getInstance()->requestVertexShaderData(shader_names.BASE_VERTEX_SHADER_NAME, &shader_byte_code, &size_shader);

	// Pre-defined value of cylinder
	const float PI = 3.141592654f;
	const float PI_HALF = PI / 2.0f;
	uint16_t segments = 16;
	uint16_t sectorCount = segments;
	float height = 2.0f;
	float radius = 1.0f;

	std::vector<vertex> vertices;

	// Generate Cylinder vertices 
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle; //rad
	int baseCenterIndex;
	int topCenterIndex;
	std::vector<float> f_vertices;
	std::vector<float> f_texCoords;

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(sin(sectorAngle)); // x
		unitCircleVertices.push_back(0);                // y
		unitCircleVertices.push_back(cos(sectorAngle)); // z
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// put side vertices to arrays of floats
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;
		float t = 1.0f - i;                              

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			float ux = unitCircleVertices[k];
			float uy = unitCircleVertices[k + 1];
			float uz = unitCircleVertices[k + 2];
			// position vector
			f_vertices.push_back(ux * radius);             // vx
			f_vertices.push_back(h);					   // vy                       
			f_vertices.push_back(uz * radius);			   // vz

			// normal vector
			//normals.push_back(ux);                       // nx
			//normals.push_back(uy);                       // ny
			//normals.push_back(uz);                       // nz
			// 
			// texture coordinate
			f_texCoords.push_back((float)j / sectorCount); // s
			f_texCoords.push_back(t);                      // t
		}
	}

	baseCenterIndex = (int)vertices.size() / 3;
	topCenterIndex = baseCenterIndex + sectorCount + 1; 

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;          
		float ny = -1 + i * 2;                          

		// center point
		f_vertices.push_back(0);       f_vertices.push_back(h);		f_vertices.push_back(0);
		//normals.push_back(0);      normals.push_back(ny);		normals.push_back(0);  
		f_texCoords.push_back(0.5f); f_texCoords.push_back(0.5f);

		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitCircleVertices[k];
			float uz = unitCircleVertices[k + 2];

			// position vector
			f_vertices.push_back(ux * radius);             // vx
			f_vertices.push_back(h);					   // vy
			f_vertices.push_back(uz * radius);			   // vz

			// normal vector
			//normals.push_back(0);                        // nx
			//normals.push_back(ny);                       // ny
			//normals.push_back(0);                        // nz
			
			// texture coordinate
			f_texCoords.push_back(-ux * 0.5f + 0.5f);      // s
			f_texCoords.push_back(-uz * 0.5f + 0.5f);      // t
		}
	}

	for (int i = 0; i < f_vertices.size() / 3; i++)
	{
		vertices.push_back({ Vector3D(f_vertices[i * 3], f_vertices[i * 3 + 1], f_vertices[i * 3 + 2]), Vector3D((f_vertices[i * 3] + 1.0f) / 2.0f, 
						  (f_vertices[i * 3 + 1] + 1.0f) / 2.0f, (f_vertices[i * 3 + 2] + 1.0f) / 2.0f) });
	}

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
	m_vb->load(vertices.data(), sizeof(vertex), vertices.size(), shader_byte_code, size_shader, GraphicsEngine::get()->getRenderSystem());
	
	// Generate Cylinder indices
	std::vector<unsigned int> indices;
	std::vector<int> lineIndices;
	int k1 = 0;                         
	int k2 = sectorCount + 1;
	
	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		indices.push_back(k1);
		indices.push_back(k1 + 1);
		indices.push_back(k2);

		indices.push_back(k2);
		indices.push_back(k1 + 1);
		indices.push_back(k2 + 1);
	}

	// indices for the base surface
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}
		else // last triangle
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}

	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(indices.data(), indices.size());

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

Cylinder::~Cylinder()
{
	AGameObject::~AGameObject();
}

void Cylinder::update(float delta_time)
{

}

void Cylinder::draw(int width, int height)
{
	ShaderNames shader_names;
	GraphicsEngine* graphics_engine = GraphicsEngine::get();
	CameraSystem* camera_system = graphics_engine->getCameraSystem();
	DeviceContextPtr device_context = graphics_engine->getRenderSystem()->getImmediateDeviceContext();
	device_context->setRenderConfig(ShaderLibrary::getInstance()->getVertexShader(shader_names.BASE_VERTEX_SHADER_NAME), ShaderLibrary::getInstance()->getPixelShader(shader_names.BASE_PIXEL_SHADER_NAME));

	constant cc = {};

	if (m_override_matrix)
		cc.m_world = m_local_matrix;
	else {
		updateLocalMatrix();
		cc.m_world = m_local_matrix;
	}

	// have to adjust for multiple viewports later
	cc.m_view = camera_system->getCurrentCameraViewMatrix();
	cc.m_proj = camera_system->getCurrentCameraProjectionMatrix();

	m_cb->update(device_context, &cc);

	device_context->setConstantBuffer(m_cb);

	device_context->setIndexBuffer(m_ib);
	device_context->setVertexBuffer(m_vb);

	device_context->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
