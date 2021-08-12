#pragma once
#include "Window.h"
#include "Matrix4x4.h"
#include "Cube.h"
#include "Prerequisites.h"
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
	static void create();
	static void destroy();

	void initialize();

	float getDeltaTime();
	void updateTimeLinear();
	void updateTimeWave();

	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	void onFocus() override;
	void onKillFocus() override;

	TexturePtr m_wood_tex;
	MeshPtr m_mesh;

private:
	std::vector<AGameObject*> m_object_list;

	SwapChainPtr m_swap_chain;
	
	VertexBufferPtr m_vb;
	VertexColorBufferPtr m_vcb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	AlphaBlendStatePtr m_abs;

	float m_time_linear;
	float m_time_wave;

	Matrix4x4 m_world_cam;
};