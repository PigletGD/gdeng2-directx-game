#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexColorBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "AlphaBlendState.h"
#include "Matrix4x4.h"
#include "Quad.h"
#include "QuadTransitionColor.h"
#include "Cube.h"
#include "Plane.h"
#include "Vertex.h"
#include "Camera.h"
#include <vector>

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();
	AppWindow(AppWindow const&) {};
	AppWindow& operator=(AppWindow const&) {};
	static AppWindow* sharedInstance;

	static AppWindow* get();
	static void intialize();
	static void destroy();

	void initializeEngine();
	void createInterface();

	float getDeltaTime();
	void updateTimeLinear();
	void updateTimeWave();

	void drawToRenderTarget(Camera* camera, UINT width, UINT height);

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;



private:
	std::vector<AGameObject*> m_object_list;
	std::vector<AGameObjectPtr> objectListSharedPtr;

	SwapChainPtr m_swap_chain;
	
	VertexBufferPtr m_vb;
	//VertexColorBufferPtr m_vcb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	AlphaBlendStatePtr m_abs;

	ID3D11RasterizerState* m_rs;

	TexturePtr m_wood_tex;
	MeshPtr m_mesh;

	float m_time_linear;
	float m_time_wave;

	Matrix4x4 m_world_cam;

	friend class UIManager;
	friend class ViewportScreen;
	friend class MenuBarScreen;
	friend class InspectorScreen;
};