#include "AppWindow.h"
#include <Windows.h>
#include "Matrix4x4.h"
#include "InputSystem.h"

struct vec3 { float x, y, z; };

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_theta;
};

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

AppWindow* AppWindow::get()
{
	return sharedInstance;
}

void AppWindow::intialize()
{
	sharedInstance = new AppWindow();
}

void AppWindow::destroy()
{
	delete sharedInstance;
}

void AppWindow::initializeEngine()
{
	GraphicsEngine::create();
	InputSystem::create();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	// DEFINING QUAD PROPERTIES
	vertex vertex_list[] = {
		// QUAD 1
		{Vector3D( 1.5f, 0.5f, 2.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.3f, 0.3f, 0.3f)}, // POS1
		{Vector3D( 2.5f, 0.5f, 2.0f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(0.3f, 0.3f, 0.0f)}, // POS2
		{Vector3D( 2.5f,-0.5f, 2.0f), Vector3D(1.0f, 0.0f, 1.0f), Vector3D(0.3f, 0.0f, 0.3f)}, // POS3
		{Vector3D( 1.5f,-0.5f, 2.0f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.3f, 0.0f, 0.0f)}, // POS4

		// QUAD 2
		{Vector3D(-0.5f, 0.5f, 1.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.3f, 0.3f, 0.3f)}, // POS1
		{Vector3D( 0.5f, 0.5f, 1.0f), Vector3D(0.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.3f, 0.3f)}, // POS2
		{Vector3D( 0.5f,-0.5f, 1.0f), Vector3D(1.0f, 0.0f, 1.0f), Vector3D(0.3f, 0.0f, 0.3f)}, // POS3
		{Vector3D(-0.5f,-0.5f, 1.0f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.3f)}, // POS4

		// QUAD 3
		{Vector3D(-2.5f, 0.5f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.3f, 0.3f, 0.3f)}, // POS1
		{Vector3D(-1.5f, 0.5f, 0.0f), Vector3D(0.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.3f, 0.3f)}, // POS2
		{Vector3D(-1.5f,-0.5f, 0.0f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(0.3f, 0.3f, 0.0f)}, // POS3
		{Vector3D(-2.5f,-0.5f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.3f, 0.0f)}, // POS4
	};
	UINT size_vertex_list = ARRAYSIZE(vertex_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

	int num_quads = ((int)ARRAYSIZE(vertex_list)) / 4;
	for (int i = 0; i < num_quads; i++) {
		quads.push_back(new Quad(vertex_list[(4 * i)], vertex_list[(4 * i) + 1], vertex_list[(4 * i) + 2], vertex_list[(4 * i) + 3]));
		quads[i]->createBuffers(shader_byte_code, size_shader);
	}

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_theta = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::updateTime() {
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? (m_new_delta - m_old_delta) * 0.001f : 0.0f;

	m_theta += 1.57 * m_delta_time;
}

void AppWindow::update()
{
	constant cc;
	cc.m_theta = m_theta;

	Matrix4x4 temp;

	m_delta_pos += m_delta_time * 0.1f;
	if (m_delta_pos > 1.0f) m_delta_pos = 0;

	m_delta_scale += m_delta_time * 2.0f;

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;
	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + 
		world_cam.getZDirection() * (m_forward * m_delta_time) +
		world_cam.getXDirection() * (m_rightward * m_delta_time);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	cc.m_view = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH(1.57f, (float)width / (float)height, 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 1.0f, 0.4f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	
	update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	for (int i = 0; i < quads.size(); i++) quads[i]->draw();

	m_swap_chain->present(true);

	updateTime();

}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->showCursor(false);
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->showCursor(true);
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W') m_forward = 1.0f;
	else if (key == 'S') m_forward = -1.0f;
	else if (key == 'A') m_rightward = -1.0f;
	else if (key == 'D') m_rightward = 1.0f;
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height * 0.5f)) * m_delta_time * 0.2f;
	m_rot_y += (mouse_pos.m_x - (width * 0.5f))* m_delta_time * 0.2f;

	InputSystem::get()->setCursorPosition(Point((int)(width * 0.5f), (int)(height * 0.5f)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
