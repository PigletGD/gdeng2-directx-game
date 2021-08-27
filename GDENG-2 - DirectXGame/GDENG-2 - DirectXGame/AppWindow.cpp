#include "AppWindow.h"
#include <Windows.h>
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "ConstantData.h"
#include "EngineTime.h"
#include "MathUtils.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "UIManager.h"

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
	EngineTime::create();

	GraphicsEngine::create();
	InputSystem::create();

	GraphicsEngine::get()->getCameraSystem()->incrementFocusCount();
	//InputSystem::get()->addListener(GraphicsEngine::get()->getCameraSystem());

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



	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene.obj");
	InputSystem::get()->addListener(m_mesh.get());
	objectListSharedPtr.push_back(m_mesh);


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	render_system->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);//VertexTransitionColorShader
	m_vs = render_system->createVertexShader(shader_byte_code, size_shader);

	/*
	{
		// PLANE OBJECT: renders first
		Plane* plane_object = new Plane("Plane 0", shader_byte_code, size_shader);
		plane_object->setScale(7, 7, 1);
		plane_object->setPosition(0.0f, -0.5f, 0.0f);
		m_object_list.push_back(plane_object);


		/* CUBE OBJECT: renders second, will render entirely in front of plane without depth stencil buffer
		for (int i = 0; i < 0; i++) {
			Cube* cube_object = new Cube("Cube " + std::to_string(i), shader_byte_code, size_shader);
			cube_object->setAnimSpeed(MathUtils::randomFloat(-3.75f, 3.75f));
			cube_object->setPosition(MathUtils::randomFloat(-6.9f, 6.9f), MathUtils::randomFloat(-6.9f, 6.9f), MathUtils::randomFloat(-6.9f, 6.9f));
			cube_object->setScale(1.0f, 1.0f, 1.0f);
			InputSystem::get()->addListener(cube_object);
			m_object_list.push_back(cube_object);
		}

		Cube* cube_object = new Cube("Cube 0", shader_byte_code, size_shader);
		cube_object->setAnimSpeed(MathUtils::randomFloat(-3.75f, 3.75f));
		cube_object->setPosition(0.0f, 0.0f, 0.0f);
		InputSystem::get()->addListener(cube_object);
		m_object_list.push_back(cube_object);

	}
	*/
	render_system->releaseCompiledShader();

	render_system->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);//PixelTransitionColorShader
	m_ps = render_system->createPixelShader(shader_byte_code, size_shader);
	render_system->releaseCompiledShader();

	constant cc;
	//cc.m_time = 0;

	m_cb = render_system->createConstantBuffer(&cc, sizeof(constant));

	m_abs = render_system->createAlphaBlendState();

	m_rs = render_system->m_rs_solid;

	camera_system->initializeInitialCamera();
}

void AppWindow::createInterface()
{
	UIManager::initialize(m_HWND);
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

void AppWindow::drawToRenderTarget(Camera* camera, UINT width, UINT height)
{
	RenderSystem* render_system = GraphicsEngine::get()->getRenderSystem();
	CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();
	DeviceContextPtr device_context = render_system->getImmediateDeviceContext();

	constant cc;
	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	static float rotation = 0;

	//rotation += 0.707f * EngineTime::getDeltaTime(); // delete me
	m_light_rot_matrix.setRotationY(rotation);
	Vector3D lightPos = {};
	lightPos.m_x = 0;
	lightPos.m_y = 2;
	lightPos.m_z = -1;
	//m_light_rot_matrix.setTranslation(lightPos);
	cc.m_light_direction = m_light_rot_matrix.getZDirection();

	//cc.m_time = m_time_linear;
	//cc.m_lerp_speed = 1.0f;
	cc.isLit = 2.0f;
	cc.m_camera_position = camera->getWorldMatrix().getTranslation() ;//camera->getWorldMatrix().getTranslation();

	cc.m_world = camera->getWorldMatrix();
	cc.m_view = camera->getViewMatrix();
	cc.m_proj = camera->getProjectionMatrix();
	

	m_cb->update(device_context, &cc);

	device_context->setConstantBuffer(m_vs, m_cb);
	device_context->setConstantBuffer(m_ps, m_cb);

	device_context->setVertexShader(m_vs);
	device_context->setPixelShader(m_ps);

	/*
	for (int i = 0; i < m_object_list.size(); i++) {
		m_object_list[i]->update(EngineTime::getDeltaTime());
		m_object_list[i]->draw(width, height, m_vs, m_ps, cc);
	}
	*/
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);
	for (int i = 0; i < objectListSharedPtr.size(); i++) {
		objectListSharedPtr[i]->update(EngineTime::getDeltaTime());
		objectListSharedPtr[i]->draw(width, height, m_vs, m_ps, cc);
	}

	//camera_system->drawGizmos(cc);
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

	camera_system->updateInputListener();

	InputSystem::get()->update();

	device_context->setAlphaBlendState(m_abs);

	device_context->clearRenderTargetColor(m_swap_chain, 1.0f, 0.4f, 0.4f, 1);

	RECT rc = getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	device_context->setViewportSize(width, height);

	device_context->setRasterizerState(m_rs);

	constant cc;
	//lighting update
	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	//static float rotation = 0;

	//rotation += 0.707f * EngineTime::getDeltaTime(); // delete me
	m_light_rot_matrix.setRotationY(0);
	Vector3D lightPos = {};
	lightPos.m_x = 0;
	lightPos.m_y = 2;
	lightPos.m_z = -1;
	//m_light_rot_matrix.setTranslation(lightPos);
	cc.m_light_direction = m_light_rot_matrix.getZDirection();

	//cc.m_time = m_time_linear;
	//cc.m_lerp_speed = 1.0f;
	cc.isLit = 1.0f;
	camera_system->updateCurrentCamera();
	cc.m_camera_position = camera_system->getCurrentCameraWorldMatrix().getTranslation();
	cc.m_world = camera_system->getCurrentCameraWorldMatrix();;
	cc.m_view = camera_system->getCurrentCameraViewMatrix();
	cc.m_proj = camera_system->getCurrentCameraProjectionMatrix();
	

	m_cb->update(device_context, &cc);

	device_context->setConstantBuffer(m_vs, m_cb);
	device_context->setConstantBuffer(m_ps, m_cb);

	device_context->setVertexShader(m_vs);
	device_context->setPixelShader(m_ps);
	{
		//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);

		//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());
		//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

		//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

		/*
		for (int i = 0; i < m_object_list.size(); i++) {
			m_object_list[i]->update(EngineTime::getDeltaTime());
			m_object_list[i]->draw(width, height, m_vs, m_ps, cc);
		}
		*/
	}
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_tex);
	for (int i = 0; i < objectListSharedPtr.size(); i++) {
		objectListSharedPtr[i]->update(EngineTime::getDeltaTime());
		objectListSharedPtr[i]->draw(width, height, m_vs, m_ps, cc);
	}

	//camera_system->drawGizmos(cc);

	UIManager::getInstance()->drawAllUI();

	device_context->setRenderTarget(m_swap_chain);

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
	GraphicsEngine::get()->getCameraSystem()->incrementFocusCount();
}

void AppWindow::onKillFocus()
{
	GraphicsEngine::get()->getCameraSystem()->decrementFocusCount();
}

void AppWindow::onSize()
{
	RECT rc = getClientWindowRect();

	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);

	GraphicsEngine::get()->getCameraSystem()->updateCurrentCameraWindowSize(rc.right - rc.left, rc.bottom - rc.top);

	onUpdate();
}
