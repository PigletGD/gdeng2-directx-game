#include "MeshObject.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "GraphicsEngine.h"
#include "ShaderLibrary.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "AppWindow.h"
#include "Mesh.h"

MeshObject::MeshObject(std::string name, std::wstring mesh_path, std::wstring texture_path, bool skip_init) :
	AGameObject(name)
{
	GraphicsEngine* graphics_engine = GraphicsEngine::get();

	m_mesh = graphics_engine->getMeshManager()->createMeshFromFile(mesh_path.c_str());
	if (!texture_path.empty()) m_texture = graphics_engine->getTextureManager()->createTextureFromFile(texture_path.c_str());

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

MeshObject::~MeshObject()
{
}

void MeshObject::update(float delta_time)
{
}

void MeshObject::draw(int width, int height)
{
	ShaderNames shader_names;
	GraphicsEngine* graphics_engine = GraphicsEngine::get();
	CameraSystem* camera_system = graphics_engine->getCameraSystem();
	DeviceContextPtr device_context = graphics_engine->getRenderSystem()->getImmediateDeviceContext();
	
	if (m_texture != nullptr) {
		device_context->setTexture(m_texture);
		device_context->setRenderConfig(ShaderLibrary::getInstance()->getVertexShader(shader_names.TEXTURED_VERTEX_SHADER_NAME),
			ShaderLibrary::getInstance()->getPixelShader(shader_names.TEXTURED_PIXEL_SHADER_NAME));
	}
	else {
		device_context->setRenderConfig(ShaderLibrary::getInstance()->getVertexShader(shader_names.BASE_VERTEX_SHADER_NAME),
			ShaderLibrary::getInstance()->getPixelShader(shader_names.BASE_PIXEL_SHADER_NAME));
	}
	
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

	device_context->setIndexBuffer(m_mesh->getIndexBuffer());
	device_context->setVertexBuffer(m_mesh->getVertexBuffer());

	device_context->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}
