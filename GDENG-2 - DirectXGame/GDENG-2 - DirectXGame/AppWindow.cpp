#include "AppWindow.h"

struct vec3 { float x, y, z; };
struct vertex { vec3 position, color; };


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

	/*
	vertex list[] = {
		//x - y - z				r - g - b
		{-0.5f,-0.5f, 0.0f,		1.0f, 0.0f, 0.0f}, // POS1
		{-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f}, // POS2
		{ 0.5f,-0.5f, 0.0f,		0.0f, 0.0f, 1.0f}, // POS3
		{ 0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f}, // POS4
	};
	*/

	// RAINBOW RECTANGLE - DISPLAYED AT TOP RIGHT OF SCREEN
	vertex output1[] = {
		//x - y - z				r - g - b
		{ 0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f}, // POS1
		{ 0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f}, // POS2
		{ 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f}, // POS3
		{ 1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f}  // POS4
	};

	// RAINBOW TRIANGLE -  DISPLAYED AT BOTTOM RIGHT OF SCREEN
	vertex output2[] = {
		//x - y - z				r - g - b
		{ 0.0f,-1.0f, 0.0f,		0.0f, 1.0f, 0.0f}, // POS1
		{ 0.5f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f}, // POS2
		{ 1.0f,-1.0f, 0.0f,		0.0f, 0.0f, 1.0f}  // POS3
	};

	// GREEN RECTANGLE - DISPLAYED AT BOTTOM LEFT OF SCREEN
	vertex output3[] = {
		//x - y - z				r - g - b
		{-1.0f,-1.0f, 0.0f,		0.0f, 1.0f, 0.0f}, // POS1
		{-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f}, // POS2
		{ 0.0f,-1.0f, 0.0f,		0.0f, 1.0f, 0.0f}, // POS3
		{ 0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f}  // POS4
	};

	// CREATING MULTIPLE BUFFERS FOR MULTIPLE TRIANGLES/QUADS
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(output1);
	m_vb2 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list2 = ARRAYSIZE(output2);
	m_vb3 = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list3 = ARRAYSIZE(output3);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(output1, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_vb2->load(output2, sizeof(vertex), size_list2, shader_byte_code, size_shader);
	m_vb3->load(output3, sizeof(vertex), size_list3, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 1, 0, 0, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb2);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb2->getSizeVertexList(), 0);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb3);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb3->getSizeVertexList(), 0);

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_vb->release();
	m_vb2->release();
	m_vb3->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();

	GraphicsEngine::get()->release();
}
