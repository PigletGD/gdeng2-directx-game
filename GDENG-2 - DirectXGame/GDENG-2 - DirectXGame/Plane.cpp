#include "Plane.h"
#include "Vector3D.h"
#include "Vertex.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"
#include "ShaderLibrary.h"

Plane::Plane(std::string name, AGameObject::PrimitiveType type, bool skip_init) : AGameObject(name, type)
{
	if (skip_init) return;

	ShaderNames shader_names;
	void* shader_byte_code = NULL;
	size_t size_shader = 0;

	ShaderLibrary::getInstance()->requestVertexShaderData(shader_names.BASE_VERTEX_SHADER_NAME, &shader_byte_code, &size_shader);

	vertex quad_list[] = {
		{Vector3D(-0.5f, 0.0f,-0.5f), Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.0f, 0.5f), Vector3D(1, 1, 1)},
		{Vector3D( 0.5f,-0.0f,-0.5f), Vector3D(1, 1, 1)},
		{Vector3D( 0.5f, 0.0f, 0.5f), Vector3D(1, 1, 1)}
	};

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer();
	m_vb->load(quad_list, sizeof(vertex), ARRAYSIZE(quad_list), shader_byte_code, size_shader, GraphicsEngine::get()->getRenderSystem());

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

Plane::~Plane()
{
}

void Plane::update(float deltaTime)
{
}

void Plane::draw(int width, int height)
{
	ShaderNames shader_names;
	GraphicsEngine* graphics_engine = GraphicsEngine::get();
	CameraSystem* camera_system = graphics_engine->getCameraSystem();
	DeviceContextPtr device_context = graphics_engine->getRenderSystem()->getImmediateDeviceContext();
	device_context->setRenderConfig(ShaderLibrary::getInstance()->getVertexShader(shader_names.BASE_VERTEX_SHADER_NAME), ShaderLibrary::getInstance()->getPixelShader(shader_names.BASE_PIXEL_SHADER_NAME));

	constant cc = {};

	//Matrix4x4 allMatrix; allMatrix.setIdentity();
	//Matrix4x4 translationMatrix; translationMatrix.setIdentity(); translationMatrix.setTranslation(getLocalPosition());
	//Matrix4x4 scaleMatrix; scaleMatrix.setIdentity(); scaleMatrix.setScale(getLocalScale());
	//Vector3D rotation = getLocalRotation();
	//Matrix4x4 zMatrix; zMatrix.setIdentity(); zMatrix.setRotationZ(rotation.m_z);
	//Matrix4x4 xMatrix; xMatrix.setIdentity(); xMatrix.setRotationX(rotation.m_x);
	//Matrix4x4 yMatrix; yMatrix.setIdentity(); yMatrix.setRotationY(rotation.m_y);

	//// multiply scale to rotation, then product to translation
	//Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	//yMatrix *= zMatrix;
	//xMatrix *= yMatrix;
	//rotMatrix *= xMatrix;
	//scaleMatrix *= rotMatrix;
	//allMatrix *= scaleMatrix;
	//allMatrix *= translationMatrix;

	//// have to adjust for multiple viewports later
	//cc.m_world = allMatrix;

	if (m_override_matrix)
		cc.m_world = m_local_matrix;
	else {
		updateLocalMatrix();
		cc.m_world = m_local_matrix;
	}

	cc.m_view = camera_system->getCurrentCameraViewMatrix();
	cc.m_proj = camera_system->getCurrentCameraProjectionMatrix();

	m_cb->update(device_context, &cc);

	device_context->setConstantBuffer(m_cb);

	device_context->setVertexBuffer(m_vb);

	device_context->drawTriangleStrip(m_vb->getListSize(), 0);
}
