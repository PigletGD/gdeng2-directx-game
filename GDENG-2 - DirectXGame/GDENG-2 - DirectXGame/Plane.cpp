#include "Plane.h"
#include "Vector3D.h"
#include "Vertex.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "VertexColorBuffer.h"
#include "MathUtils.h"

Plane::Plane(std::string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
	vertex_color quad[] = {
		{Vector3D(-0.5f,-0.5f, 0.0f), Vector3D(-0.5f,-0.5f, 0.0f), Vector3D(1, 1, 1), Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, 0.0f), Vector3D(-0.5f, 0.5f, 0.0f), Vector3D(1, 1, 1), Vector3D(1, 1, 1)},
		{Vector3D( 0.5f,-0.5f, 0.0f), Vector3D( 0.5f,-0.5f, 0.0f), Vector3D(1, 1, 1), Vector3D(1, 1, 1)},
		{Vector3D( 0.5f, 0.5f, 0.0f), Vector3D( 0.5f, 0.5f, 0.0f), Vector3D(1, 1, 1), Vector3D(1, 1, 1)}
	};

	m_vcb = GraphicsEngine::get()->getRenderSystem()->createVertexColorBuffer(quad, sizeof(vertex_color), ARRAYSIZE(quad), shaderByteCode, sizeShader);

	constant cc;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	setRotation(MathUtils::DegToRad(90), 0, 0);
}

Plane::~Plane()
{
}

void Plane::update(float deltaTime)
{
}

void Plane::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContextPtr deviceContext = graphEngine->getRenderSystem()->getImmediateDeviceContext();

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

	deviceContext->setVertexBuffer(m_vcb);

	deviceContext->drawTriangleStrip(m_vcb->getSizeVertexList(), 0);
}
