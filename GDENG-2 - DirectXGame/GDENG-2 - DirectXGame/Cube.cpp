#include "Cube.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "Vertex.h"
#include "ConstantData.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"
#include "ShaderLibrary.h"

Cube::Cube(std::string name, AGameObject::PrimitiveType type, bool skip_init) :
	AGameObject(name, type)
{
	if (skip_init) return;

	ShaderNames shader_names;
	void* shader_byte_code = NULL;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shader_names.BASE_VERTEX_SHADER_NAME, &shader_byte_code, &size_shader);

	//create buffers for drawing. vertex data that needs to be drawn are temporarily placed here.
	vertex quad_list[] = {
		//X, Y, Z
		//FRONT FACE
		{ Vector3D(-0.5f,-0.5f,-0.5f), Vector3D(1,0,0) },
		{ Vector3D(-0.5f,0.5f,-0.5f), Vector3D(1,1,0) },
		{ Vector3D(0.5f,0.5f,-0.5f), Vector3D(1,1,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f), Vector3D(1,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f), Vector3D(0,1,0) },
		{ Vector3D(0.5f,0.5f,0.5f), Vector3D(0,1,1) },
		{ Vector3D(-0.5f,0.5f,0.5f), Vector3D(0,1,1) },
		{ Vector3D(-0.5f,-0.5f,0.5f), Vector3D(0,1,0) },
	};

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
	m_vb->load(quad_list, sizeof(vertex), ARRAYSIZE(quad_list), shader_byte_code, size_shader, GraphicsEngine::get()->getRenderSystem());

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};
	//this->indexBuffer = GraphicsEngine::getInstance()->createIndexBuffer();
	//this->indexBuffer->load(indexList, ARRAYSIZE(indexList));

	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, ARRAYSIZE(index_list));

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	//create constant buffer
	//CBData cbData = {};
	//cbData.time = 0;
	//this->constantBuffer = GraphicsEngine::getInstance()->createConstantBuffer();
	//this->constantBuffer->load(&cbData, sizeof(CBData));
}

Cube::~Cube()
{
	
}

void Cube::update(float delta_time)
{
	
}

void Cube::draw(int width, int height)
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