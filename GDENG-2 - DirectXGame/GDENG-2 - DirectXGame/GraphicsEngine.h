#pragma once
#include <d3d11.h>

#include "RenderSystem.h"
#include "Prerequisites.h"
#include "TextureManager.h"
#include "MeshManager.h"

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
	MeshManager* getMeshManager();
	void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size);

	static GraphicsEngine* get();
	static void create();
	static void release();

private:
	static GraphicsEngine* m_engine;
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_tex_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};