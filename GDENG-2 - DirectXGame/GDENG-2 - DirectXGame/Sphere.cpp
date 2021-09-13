#include "Sphere.h"
#include "GraphicsEngine.h"
#include "ShaderLibrary.h"
#include "Vertex.h"
#include <Windows.h>

#include "ConstantBuffer.h"
#include "DeviceContext.h"

Sphere::Sphere(std::string name)
	: AGameObject(name)
{
	ShaderNames shader_names;
	void* shader_byte_code = NULL;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shader_names.BASE_VERTEX_SHADER_NAME, &shader_byte_code, &size_shader);

	const float PI = 3.141592654f;
	const float PI_HALF = PI / 2.0f;
	uint16_t segments = 16;
	uint16_t rings = 32;
	uint16_t sectorCount = segments;
	uint16_t stackCount = (rings + 1);
	std::vector<vertex> vertices;

	// Generate sphere vertices
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI_HALF - i * stackStep;
		float xz = cosf(stackAngle);
		float y = sinf(stackAngle);
		
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;
			
			float x = xz * cosf(sectorAngle);
			float z = xz * sinf(sectorAngle);
			vertices.push_back({ Vector3D(x, y, z), Vector3D((x + 1.0f) / 2.0f, (y + 1.0f) / 2.0f, (z + 1.0f) / 2.0f) });
		}
	}

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
	m_vb->load(vertices.data(), sizeof(vertex), vertices.size(), shader_byte_code, size_shader, GraphicsEngine::get()->getRenderSystem());

	std::vector<unsigned int> indices;
	std::vector<int> lineIndices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			// store indices for lines
			// vertical lines for all stacks, k1 => k2
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}
	
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(indices.data(), indices.size());

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

Sphere::~Sphere()
{
	
}

void Sphere::update(float delta_time)
{
	
}

void Sphere::draw(int width, int height)
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
