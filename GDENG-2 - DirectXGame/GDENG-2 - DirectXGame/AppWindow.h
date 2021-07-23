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
#include "InputListener.h"
#include "Matrix4x4.h"
#include "Quad.h"
#include "QuadTransitionColor.h"

#include "Vertex.h"

#include <vector>

class AppWindow : public Window, public InputListener
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

	float getDeltaTime();
	void updateTimeLinear();
	void updateTimeWave();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	TexturePtr m_wood_tex;
	MeshPtr m_mesh;
private:
	QuadTransitionColor* m_quad_color_one;
	QuadTransitionColor* m_quad_color_two;

	SwapChainPtr m_swap_chain;

	VertexBufferPtr m_vb;
	VertexColorBufferPtr m_vcb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	AlphaBlendStatePtr m_abs;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_time_linear;
	float m_time_wave;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1.0f;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Matrix4x4 m_world_cam;
};