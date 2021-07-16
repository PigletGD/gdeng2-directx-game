#pragma once
#include <d3d11.h>

#include "RenderSystem.h"
#include "Prerequisites.h"
#include "TextureManager.h"

#include <d3d11.h>

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();
	GraphicsEngine(GraphicsEngine const&) {};
	GraphicsEngine& operator=(GraphicsEngine const&) {};
public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	static GraphicsEngine* m_engine;
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_tex_manager = nullptr;
};