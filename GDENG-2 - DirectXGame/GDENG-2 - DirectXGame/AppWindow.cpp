#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

#include <iostream>
#include <string>

struct vec3 { float x, y, z; };
struct vertex { Vector3D position, color, color1; };

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_theta;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::updateTime() {
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? (m_new_delta - m_old_delta) * 0.001f : 0.0f;

	m_theta += 1.57 * m_delta_time;
}

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_theta = m_theta;

	Matrix4x4 temp;

	m_delta_pos += m_delta_time * 0.1f;
	if (m_delta_pos > 1.0f) m_delta_pos = 0;

	m_delta_scale += m_delta_time * 2.0f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(1, 1, 0), (sin(m_delta_scale) + 1.0f) * 0.5f));
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
	//cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(1, 1, 1));

	temp.setIdentity();
	temp.setRotationZ(m_delta_scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_delta_scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_delta_scale);
	cc.m_world *= temp;

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

	vertex vertex_list[] = {
		// FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f)}, // POS1
		{Vector3D(-0.5f, 0.5f,-0.5f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(1.0f, 0.0f, 1.0f)}, // POS2
		{Vector3D( 0.5f, 0.5f,-0.5f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(1.0f, 0.0f, 0.0f)}, // POS3
		{Vector3D( 0.5f,-0.5f,-0.5f), Vector3D(0.0f, 1.0f, 1.0f), Vector3D(1.0f, 1.0f, 1.0f)}, // POS4
		// BACK FACE
		{Vector3D( 0.5f,-0.5f, 0.5f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f)}, // POS5
		{Vector3D( 0.5f, 0.5f, 0.5f), Vector3D(1.0f, 0.0f, 1.0f), Vector3D(0.0f, 1.0f, 1.0f)}, // POS6
		{Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f)}, // POS7
		{Vector3D(-0.5f,-0.5f, 0.5f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(1.0f, 1.0f, 0.0f)}, // POS8
	};

	// CREATING MULTIPLE BUFFERS FOR MULTIPLE TRIANGLES/QUADS
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_vertex_list = ARRAYSIZE(vertex_list);

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

	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_theta = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 1.0f, 0.4f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	
	updateQuadPosition();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	m_swap_chain->present(true);

	updateTime();

}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_vb->release();
	m_ib->release();
	m_cb->release();

	m_swap_chain->release();
	
	m_vs->release();
	m_ps->release();

	GraphicsEngine::get()->release();
}
