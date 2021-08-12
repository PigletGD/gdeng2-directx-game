#include "AppWindow.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "CameraSystem.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "ConstantBuffer.h"
#include "ConstantData.h"
#include "Matrix4x4.h"
#include "MathUtils.h"
#include "Plane.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow::AppWindow()
{
	
}

AppWindow* AppWindow::get()
{
	return sharedInstance;
}

void AppWindow::create()
{
	sharedInstance = new AppWindow();
}

void AppWindow::destroy()
{
	delete sharedInstance;
}

void AppWindow::initialize()
{
	InputSystem::get()->addListener(GraphicsEngine::get()->getCameraSystem());

	RenderSystem* render_system = GraphicsEngine::get()->getRenderSystem();
	CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();

	camera_system->initializeGizmoTexture();

	RECT rc = this->getClientWindowRect();
	m_swap_chain = render_system->createSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

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

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	render_system->compileVertexShader(L"VertexTransitionColorShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = render_system->createVertexShader(shader_byte_code, size_shader);

	// PLANE OBJECT: renders first
	Plane* plane_object = new Plane("Plane 0", shader_byte_code, size_shader);
	plane_object->setScale(7, 7, 1);
	m_object_list.push_back(plane_object);

	/* CUBE OBJECT: renders second, will render entirely in front of plane without depth stencil buffer
	for (int i = 0; i < 0; i++) {
		Cube* cube_object = new Cube("Cube " + std::to_string(i), shader_byte_code, size_shader);
		cube_object->setAnimSpeed(MathUtils::randomFloat(-3.75f, 3.75f));
		cube_object->setPosition(MathUtils::randomFloat(-6.9f, 6.9f), MathUtils::randomFloat(-6.9f, 6.9f), MathUtils::randomFloat(-6.9f, 6.9f));
		cube_object->setScale(1.0f, 1.0f, 1.0f);
		InputSystem::get()->addListener(cube_object);
		m_object_list.push_back(cube_object);
	}*/

	Cube* cube_object = new Cube("Cube 0", shader_byte_code, size_shader);
	cube_object->setAnimSpeed(MathUtils::randomFloat(-3.75f, 3.75f));
	cube_object->setScale(1.0f, 1.0f, 1.0f);
	cube_object->setPosition(0.0f, 0.0f, 2.0f);
	InputSystem::get()->addListener(cube_object);
	m_object_list.push_back(cube_object);

	render_system->releaseCompiledShader();

	render_system->compilePixelShader(L"PixelTransitionColorShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = render_system->createPixelShader(shader_byte_code, size_shader);
	render_system->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = render_system->createConstantBuffer(&cc, sizeof(constant));

	m_abs = render_system->createAlphaBlendState();

	camera_system->initializeInitialCamera();
}

float AppWindow::getDeltaTime()
{
	return EngineTime::getDeltaTime();
}

void AppWindow::updateTimeLinear()
{
	m_time_linear += EngineTime::getDeltaTime();
}

void AppWindow::updateTimeWave()
{
	m_time_wave += EngineTime::getDeltaTime() * ((sin(m_time_linear) + 1.0f) / 2.0f) * 10.0f; // delta * sin wave time * max speed
}

void AppWindow::onCreate()
{
	Window::onCreate();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	RenderSystem* render_system = GraphicsEngine::get()->getRenderSystem();
	CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();
	DeviceContextPtr device_context = render_system->getImmediateDeviceContext();

	InputSystem::get()->update();

	// Clear
	device_context->setAlphaBlendState(m_abs);
	device_context->clearRenderTargetColor(m_swap_chain, 1.0f, 0.4f, 0.4f, 1);

	// ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);
	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	ImGui::Begin("Test");
	ImGui::Text("Hello world");
	ImGui::End();
	
	RECT rc = getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	
	device_context->setViewportSize(width, height);

	constant cc;
	cc.m_time = m_time_linear;
	cc.m_lerp_speed = 1.0f;

	camera_system->updateCurrentCamera();

	cc.m_world = camera_system->getCurrentCameraWorldMatrix();;
	cc.m_view = camera_system->getCurrentCameraViewMatrix();
	cc.m_proj = camera_system->getCurrentCameraProjectionMatrix();

	m_cb->update(device_context, &cc);

	device_context->setConstantBuffer(m_vs, m_cb);
	device_context->setConstantBuffer(m_ps, m_cb);

	device_context->setVertexShader(m_vs);
	device_context->setPixelShader(m_ps);

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	GraphicsEngine::get()->getRenderSystem()->setSolidRasterizerState();
	
	for (int i = 0; i < m_object_list.size(); i++)
	{
		m_object_list[i]->update(EngineTime::getDeltaTime());
		m_object_list[i]->draw(width, height, m_vs, m_ps, cc);
	}

	camera_system->drawGizmos(cc);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	
	m_swap_chain->present(true);

	updateTimeLinear();
	updateTimeWave();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(GraphicsEngine::get()->getCameraSystem());
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(GraphicsEngine::get()->getCameraSystem());
}

AppWindow::~AppWindow()
{

}
