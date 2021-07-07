#include "AppWindow.h"
#include <Windows.h>

struct vec3 { float x, y, z; };
struct vertex { vec3 position, position1, color, color1; };

__declspec(align(16))
struct constant { float m_theta; };

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] = {
		//x - y - z				r - g - b
		{-0.5f,-0.5f, 0.0f,		-0.32f,-0.11f, 0.0f,		0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f}, // POS1
		{-0.5f, 0.5f, 0.0f,		-0.11f, 0.78f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f}, // POS2
		{ 0.5f,-0.5f, 0.0f,		 0.75f,-0.73f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f}, // POS3
		{ 0.5f, 0.5f, 0.0f,		 0.88f, 0.77f, 0.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f}, // POS4
	};

	// CREATING MULTIPLE BUFFERS FOR MULTIPLE TRIANGLES/QUADS
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
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

	unsigned long new_time = 0;
	if (m_old_time) new_time = ::GetTickCount64() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount64();

	m_theta += 1.57f * m_delta_time;
	constant cc;
	cc.m_theta = m_theta;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();

	GraphicsEngine::get()->release();
}
