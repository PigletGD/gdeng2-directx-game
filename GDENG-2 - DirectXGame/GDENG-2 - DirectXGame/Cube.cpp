#include "Cube.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "Vertex.h"
#include "ConstantData.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"
#include "ShaderLibrary.h"

/*
Cube::Cube(std::string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
	// random color
	//Vector3D color1(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));
	//Vector3D color2(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));
	//Vector3D color3(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));
	//Vector3D color4(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));

	Vector3D color1(1, 0, 1);
	Vector3D color2(1, 1, 0);
	Vector3D color3(0, 0, 1);
	Vector3D color4(1, 0, 0);
	Vector3D color5(0, 1, 0);

	vertex_color quad_list[] = {
		// x, y, z
		// front face
		{Vector3D(-0.5f,-0.5f,-0.5f), Vector3D(-0.5f,-0.5f,-0.5f), color1, color1},
		{Vector3D(-0.5f, 0.5f,-0.5f), Vector3D(-0.5f, 0.5f,-0.5f), color2, color2},
		{Vector3D( 0.5f, 0.5f,-0.5f), Vector3D( 0.5f, 0.5f,-0.5f), color2, color2},
		{Vector3D( 0.5f,-0.5f,-0.5f), Vector3D( 0.5f,-0.5f,-0.5f), color1, color1},

		// back face
		{Vector3D( 0.5f,-0.5f, 0.5f), Vector3D( 0.5f,-0.5f, 0.5f), color3, color3},
		{Vector3D( 0.5f, 0.5f, 0.5f), Vector3D( 0.5f, 0.5f, 0.5f), color4, color4},
		{Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(-0.5f, 0.5f, 0.5f), color5, color5},
		{Vector3D(-0.5f,-0.5f, 0.5f), Vector3D(-0.5f,-0.5f, 0.5f), color3, color3}
	};

	m_vcb = GraphicsEngine::get()->getRenderSystem()->createVertexColorBuffer(quad_list, sizeof(vertex_color), ARRAYSIZE(quad_list), shaderByteCode, sizeShader);

	unsigned int index_list[] =
	{
		// FRONT SIDE
		0, 1, 2, // TRI1
		2, 3, 0, // TRI2
		// BACK SIDE
		4, 5, 6, // TRI3
		6, 7, 4, // TRI4
		// TOP SIDE
		1, 6, 5, // TRI5
		5, 2, 1, // TRI6
		// BOTTOM SIDE
		7, 0, 3, // TRI7
		3, 4, 7, // TRI8
		// RIGHT SIDE
		3, 2, 5, // TRI9
		5, 4, 3, // TRI10
		// LEFT SIDE
		7, 6, 1, // TRI11
		1, 0, 7  // TRI12
	};

	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, ARRAYSIZE(index_list));

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}
*/

Cube::Cube(std::string name, bool skip_init) :
	AGameObject(name)
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
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0) },
		{Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0) },
		{Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0) },

		//BACK FACE
		{Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0) },
		{Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1) },
		{Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1) },
		{Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0) },
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
	AGameObject::~AGameObject();
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

	// prepare matrices
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
	
	// have to adjust for multiple viewports later
	cc.m_world = allMatrix;
	cc.m_view = camera_system->getCurrentCameraViewMatrix();
	cc.m_proj = camera_system->getCurrentCameraProjectionMatrix();

	m_cb->update(device_context, &cc);

	device_context->setConstantBuffer(m_cb);

	device_context->setIndexBuffer(m_ib);
	device_context->setVertexBuffer(m_vb);

	device_context->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}