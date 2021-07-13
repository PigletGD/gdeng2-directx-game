#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();
	AppWindow(AppWindow const&) {};
	AppWindow& operator=(AppWindow const&) {};
	static AppWindow* sharedInstance;

	static AppWindow* get();
	static void initialize();
	static void destroy();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	void initializeEngine();

private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexBuffer* m_vb2;
	VertexBuffer* m_vb3;
	VertexShader* m_vs;
	PixelShader* m_ps;
};

