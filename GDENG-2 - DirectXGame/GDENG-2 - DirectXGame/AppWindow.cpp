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
#include "ShaderLibrary.h"
#include "GameObjectManager.h"
#include "ComponentSystem.h"
#include "PhysicsSystem.h"
#include "EngineBackend.h"
#include "ActionHistory.h"
#include "Sphere.h"
#include "Cylinder.h"

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
	EngineBackend::initialize();
	EngineTime::initialize();

	GraphicsEngine::create();
	InputSystem::create();
	ShaderLibrary::initialize();

	GraphicsEngine::get()->getCameraSystem()->setMainWindowFocus(true);

	RenderSystem* render_system = GraphicsEngine::get()->getRenderSystem();
	CameraSystem* camera_system = GraphicsEngine::get()->getCameraSystem();

	camera_system->initializeInitialCamera();

	//camera_system->initializeGizmoTexture();

	RECT rc = this->getClientWindowRect();
	m_swap_chain = render_system->createSwapChain(this->m_HWND, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	constant cc;
	cc.m_time = 0;

	m_cb = render_system->createConstantBuffer(&cc, sizeof(constant));

	m_abs = render_system->createAlphaBlendState();

	m_ss = render_system->createSamplerState();

	m_rs = render_system->m_rs_solid;

	GameObjectManager::initialize();
	ComponentSystem::initialize();
	ActionHistory::initialize();

	// Load initial object
	GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\teapot.obj");
	GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\bunny.obj");
	//GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\armadillo.obj");

	//Sphere* sphere = new Sphere("Sphere");
	//GameObjectManager::getInstance()->addObject(sphere);

	Cylinder* cylinder = new Cylinder("Cylinder");
	GameObjectManager::getInstance()->addObject(cylinder);

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
	cc.m_time = m_time_linear;
	cc.m_lerp_speed = 1.0f;

	cc.m_world = camera->getWorldMatrix();
	cc.m_view = camera->getViewMatrix();
	cc.m_proj = camera->getProjectionMatrix();

	m_cb->update(device_context, &cc);

	GameObjectManager::getInstance()->renderAll(width, height);

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

	updateTimeLinear();
	updateTimeWave();

	camera_system->updateInputListener();

	InputSystem::get()->update();

	device_context->setAlphaBlendState(m_abs);

	device_context->clearRenderTargetColor(m_swap_chain, 1.0f, 0.4f, 0.4f, 1);

	RECT rc = getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	device_context->setViewportSize(width, height);

	device_context->setRasterizerState(m_rs);

	device_context->setSamplerState(m_ss);

	camera_system->updateCurrentCamera();
	camera_system->setCurrentToMainViewCamera();

	EngineBackend* backend = EngineBackend::getInstance();
	if (backend->getMode() == EngineBackend::EditorMode::PLAY) {
		ComponentSystem::getInstance()->getPhysicsSystem()->updateAllComponents();
		GameObjectManager::getInstance()->updateAll();
	}
	else if (backend->getMode() == EngineBackend::EditorMode::EDITOR) {
		GameObjectManager::getInstance()->updateAll();

	}
	else if (backend->getMode() == EngineBackend::EditorMode::PAUSED) {
		if (backend->insideFrameStep()) {
			ComponentSystem::getInstance()->getPhysicsSystem()->updateAllComponents();
			GameObjectManager::getInstance()->updateAll();
			backend->endFrameStep();
		}
	}

	GameObjectManager::getInstance()->renderAll(width, height);

	//camera_system->drawGizmos(cc);

	UIManager::getInstance()->drawAllUI();

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	ComponentSystem::destroy();
	GameObjectManager::destroy();
	ShaderLibrary::destroy();

	Window::onDestroy();
}

void AppWindow::onFocus()
{
	GraphicsEngine::get()->getCameraSystem()->setMainWindowFocus(true);
}

void AppWindow::onKillFocus()
{
	GraphicsEngine::get()->getCameraSystem()->setMainWindowFocus(false);
}

void AppWindow::onSize()
{
	RECT rc = getClientWindowRect();

	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);

	GraphicsEngine::get()->getCameraSystem()->updateCurrentCameraWindowSize(rc.right - rc.left, rc.bottom - rc.top);

	onUpdate();
}
