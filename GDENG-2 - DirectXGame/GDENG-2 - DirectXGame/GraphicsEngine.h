#pragma once
#include <d3d11.h>

#include "RenderSystem.h"
#include "Prerequisites.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();
	GraphicsEngine(GraphicsEngine const&) {};
	GraphicsEngine& operator=(GraphicsEngine const&) {};
public:
	RenderSystem* getRenderSystem();

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	static GraphicsEngine* m_engine;
	RenderSystem* m_render_system = nullptr;
};