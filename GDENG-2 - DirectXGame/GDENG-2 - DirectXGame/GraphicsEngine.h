#pragma once
#include <d3d11.h>

#include "RenderSystem.h"
#include "CameraSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

#include "Prerequisites.h"

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
	CameraSystem* getCameraSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	static GraphicsEngine* m_engine;

	RenderSystem* m_render_system = nullptr;
	CameraSystem* m_camera_system = nullptr;
	TextureManager* m_tex_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;
};