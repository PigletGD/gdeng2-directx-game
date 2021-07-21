#include "AppWindow.h"
#include <Windows.h>
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "ConstantData.h"

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
	InputSystem::get()->addListener(GraphicsEngine::get()->getCameraSystem());
	InputSystem::get()->showCursor(false);

	GraphicsEngine::get()->getCameraSystem()->initializeGizmoTexture();

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	/*
	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\teapot.obj");

	Vector3D position_list[] =
	{
		// FRONT FACE
		Vector3D(-0.5f,-0.5f,-0.5f), // POS1
		Vector3D(-0.5f, 0.5f,-0.5f), // POS2
		Vector3D(0.5f, 0.5f,-0.5f), // POS3
		Vector3D(0.5f,-0.5f,-0.5f), // POS4
		// BACK FACE
		Vector3D(0.5f,-0.5f, 0.5f), // POS5
		Vector3D(0.5f, 0.5f, 0.5f), // POS6
		Vector3D(-0.5f, 0.5f, 0.5f), // POS7
		Vector3D(-0.5f,-0.5f, 0.5f), // POS8
	};

	Vector2D texcoord_list[] =
	{
		Vector2D(0.0f,0.0f),
		Vector2D(0.0f,1.0f),
		Vector2D(1.0f,1.0f),
		Vector2D(1.0f,0.0f)
	};

	vertex vertex_list[] =
	{
		//X - Y - Z
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
	UINT size_vertex_list = ARRAYSIZE(vertex_list);

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
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->getCameraSystem()->createCameraBuffers(shader_byte_code, size_shader);

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();
	*/
	
	vertex_color vertex_list[] = {
		//x - y - z				r - g - b
		{Vector3D(-0.5f,-0.5f, 0.0f),	Vector3D(-0.32f,-0.11f, 0.0f),	Vector3D(0.0f, 0.0f, 0.0f),	Vector3D(0.0f, 1.0f, 0.0f)}, // POS1
		{Vector3D(-0.5f, 0.5f, 0.0f),	Vector3D(-0.11f, 0.78f, 0.0f),	Vector3D(1.0f, 1.0f, 0.0f),	Vector3D(0.0f, 1.0f, 1.0f)}, // POS2
		{Vector3D( 0.5f,-0.5f, 0.0f),	Vector3D( 0.75f,-0.73f, 0.0f),	Vector3D(0.0f, 0.0f, 1.0f),	Vector3D(1.0f, 0.0f, 0.0f)}, // POS3
		{Vector3D( 0.5f, 0.5f, 0.0f),	Vector3D( 0.88f, 0.77f, 0.0f),	Vector3D(1.0f, 1.0f, 1.0f),	Vector3D(0.0f, 0.0f, 1.0f)}  // POS4
	};
	UINT size_vertex_list = ARRAYSIZE(vertex_list);

	m_quad_color = new QuadTransitionColor(vertex_list[0], vertex_list[1], vertex_list[2], vertex_list[3]);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexTransitionColorShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);

	m_quad_color->createBuffers(shader_byte_code, size_shader);

	GraphicsEngine::get()->getCameraSystem()->createCameraBuffers(shader_byte_code, size_shader);

	//m_vcb = GraphicsEngine::get()->getRenderSystem()->createVertexColorBuffer(vertex_list, sizeof(vertex_color), size_vertex_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelTransitionColorShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.m_theta = 0;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	m_abs = GraphicsEngine::get()->getRenderSystem()->createAlphaBlendState();

	GraphicsEngine::get()->getCameraSystem()->setCameraShaders();
}

float AppWindow::getDeltaTime()
{
	return m_delta_time;
}

void AppWindow::updateTime() {
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? (m_new_delta - m_old_delta) * 0.001f : 0.0f;

	m_theta += 1.57 * m_delta_time;
}

void AppWindow::update()
{
	
}

void AppWindow::onCreate()
{
	Window::onCreate();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setAlphaBlendState(m_abs);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 1.0f, 0.4f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	constant cc;
	cc.m_theta = m_theta;

	GraphicsEngine::get()->getCameraSystem()->updateCurrentCamera();

	cc.m_world.setIdentity();
	cc.m_view = GraphicsEngine::get()->getCameraSystem()->getCurrentCameraView();
	cc.m_proj = GraphicsEngine::get()->getCameraSystem()->getCurrentCameraProjection();

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	m_quad_color->draw();

	GraphicsEngine::get()->getCameraSystem()->drawGizmos(m_vs, m_ps, cc);

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
	InputSystem::get()->addListener(GraphicsEngine::get()->getCameraSystem());
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->showCursor(true);
	InputSystem::get()->removeListener(GraphicsEngine::get()->getCameraSystem());
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
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
