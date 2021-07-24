#include "Cube.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "SwapChain.h"
#include "Vertex.h"
#include "ConstantData.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"

Cube::Cube(std::string name, void* shaderByteCode, size_t sizeShader) : AGameObject(name)
{
	// random color
	//Vector3D color1(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));
	//Vector3D color2(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));
	//Vector3D color3(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));
	//Vector3D color4(MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f), MathUtils::randomFloat(0.0f, 1.0f));

	Vector3D color1(1, 0, 0);
	Vector3D color2(1, 1, 0);
	Vector3D color3(0, 1, 0);
	Vector3D color4(0, 1, 1);

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
		{Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(-0.5f, 0.5f, 0.5f), color4, color4},
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

Cube::~Cube()
{
}

void Cube::update(float deltaTime)
{
	m_delta_time = deltaTime;
}

void Cube::draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc)
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

	deviceContext->setIndexBuffer(m_ib);
	deviceContext->setVertexBuffer(m_vcb);

	deviceContext->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}

void Cube::setAnimSpeed(float speed)
{
	m_speed = speed;
}

void Cube::onKeyDown(int key)
{
	if (key == 'R') {
		m_ticks += m_delta_time;

		float rotSpeed = m_ticks * m_speed;
		this->setRotation(rotSpeed, rotSpeed, rotSpeed);
	}
	else if (key == 'F') {
		m_ticks -= m_delta_time;

		float rotSpeed = m_ticks * m_speed;
		this->setRotation(rotSpeed, rotSpeed, rotSpeed);
	}
}

void Cube::onKeyUp(int key)
{
}

void Cube::onMouseMove(const Point& mouse_pos)
{
}

void Cube::onLeftMouseDown(const Point& mouse_pos)
{
}

void Cube::onLeftMouseUp(const Point& mouse_pos)
{
}

void Cube::onRightMouseDown(const Point& mouse_pos)
{
}

void Cube::onRightMouseUp(const Point& mouse_pos)
{
}
