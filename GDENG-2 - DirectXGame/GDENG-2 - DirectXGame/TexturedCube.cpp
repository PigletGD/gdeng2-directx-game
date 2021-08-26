#include "TexturedCube.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "Vertex.h"
#include "ConstantData.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"
#include "ShaderLibrary.h"

TexturedCube::TexturedCube(String name) : Cube(name, true)
{
	ShaderNames shaderNames;
	void* shader_byte_code = NULL;
	size_t size_shader = 0;
	ShaderLibrary::getInstance()->requestVertexShaderData(shaderNames.TEXTURED_VERTEX_SHADER_NAME, &shader_byte_code, &size_shader);

	//create buffers for drawing. vertex data that needs to be drawn are temporarily placed here.
	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f) },
		{ Vector3D(0.5f,0.5f,0.5f) },
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f) }
	};

	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	textured_vertex quad_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },


		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },


		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }

	};

	m_vb = GraphicsEngine::get()->getRenderSystem()->createTexturedVertexBuffer();
	m_vb->load(quad_list, sizeof(textured_vertex), ARRAYSIZE(quad_list), shader_byte_code, size_shader, GraphicsEngine::get()->getRenderSystem());

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};

	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, ARRAYSIZE(index_list));

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

TexturedCube::~TexturedCube()
{
}

void TexturedCube::draw(int width, int height)
{
	ShaderNames shader_names;
	GraphicsEngine* graphics_engine = GraphicsEngine::get();
	CameraSystem* camera_system = graphics_engine->getCameraSystem();
	DeviceContextPtr device_context = graphics_engine->getRenderSystem()->getImmediateDeviceContext();

	TexturePtr wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wood.jpg");

	device_context->setTexture(wood_tex);
	device_context->setRenderConfig(ShaderLibrary::getInstance()->getVertexShader(shader_names.TEXTURED_VERTEX_SHADER_NAME), ShaderLibrary::getInstance()->getPixelShader(shader_names.TEXTURED_PIXEL_SHADER_NAME));

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
