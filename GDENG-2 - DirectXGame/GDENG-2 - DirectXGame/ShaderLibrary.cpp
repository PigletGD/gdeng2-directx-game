#include "ShaderLibrary.h"

#include <iostream>
#include <string>

#include "GraphicsEngine.h"
#include "VertexShader.h"
#include "PixelShader.h"

ShaderLibrary* ShaderLibrary::m_shared_instance = NULL;

ShaderLibrary* ShaderLibrary::getInstance()
{
	return m_shared_instance;
}

ShaderLibrary::ShaderLibrary()
{
	//initialize and load all shaders for use
	RenderSystem* render_system = GraphicsEngine::get()->getRenderSystem();

	ShaderNames shaderNames;
	ShaderData shaderData;

	// base shaders
	render_system->compileVertexShader(shaderNames.BASE_VERTEX_SHADER_NAME.c_str(), "vsmain", &shaderData.shader_byte_code, &shaderData.size_shader);
	m_active_vertex_shaders[shaderNames.BASE_VERTEX_SHADER_NAME] = render_system->createVertexShader(shaderData.shader_byte_code, shaderData.size_shader);
	render_system->releaseCompiledShader();
	render_system->compilePixelShader(shaderNames.BASE_PIXEL_SHADER_NAME.c_str(), "psmain", &shaderData.shader_byte_code, &shaderData.size_shader);
	m_active_pixel_shaders[shaderNames.BASE_PIXEL_SHADER_NAME] = render_system->createPixelShader(shaderData.shader_byte_code, shaderData.size_shader);
	render_system->releaseCompiledShader();

	// lerp shaders
	render_system->compileVertexShader(shaderNames.LERP_VERTEX_SHADER_NAME.c_str(), "vsmain", &shaderData.shader_byte_code, &shaderData.size_shader);
	m_active_vertex_shaders[shaderNames.LERP_VERTEX_SHADER_NAME] = render_system->createVertexShader(shaderData.shader_byte_code, shaderData.size_shader);
	render_system->releaseCompiledShader();
	render_system->compilePixelShader(shaderNames.LERP_PIXEL_SHADER_NAME.c_str(), "psmain", &shaderData.shader_byte_code, &shaderData.size_shader);
	m_active_pixel_shaders[shaderNames.LERP_PIXEL_SHADER_NAME] = render_system->createPixelShader(shaderData.shader_byte_code, shaderData.size_shader);
	render_system->releaseCompiledShader();

	// textured shaders
	render_system->compileVertexShader(shaderNames.TEXTURED_VERTEX_SHADER_NAME.c_str(), "vsmain", &shaderData.shader_byte_code, &shaderData.size_shader);
	m_active_vertex_shaders[shaderNames.TEXTURED_VERTEX_SHADER_NAME] = render_system->createVertexShader(shaderData.shader_byte_code, shaderData.size_shader);
	render_system->releaseCompiledShader();
	render_system->compilePixelShader(shaderNames.TEXTURED_PIXEL_SHADER_NAME.c_str(), "psmain", &shaderData.shader_byte_code, &shaderData.size_shader);
	m_active_pixel_shaders[shaderNames.TEXTURED_PIXEL_SHADER_NAME] = render_system->createPixelShader(shaderData.shader_byte_code, shaderData.size_shader);
	render_system->releaseCompiledShader();

	std::cout << "Shader library initialized" << std::endl;
}

ShaderLibrary::~ShaderLibrary()
{
	m_active_vertex_shaders.clear();
	m_active_pixel_shaders.clear();
}

void ShaderLibrary::initialize()
{
	m_shared_instance = new ShaderLibrary();
}

void ShaderLibrary::destroy()
{
	delete m_shared_instance;
}

void ShaderLibrary::requestVertexShaderData(String vertex_shader_name, void** shader_byte_code, size_t* size_shader)
{
	RenderSystem* render_system = GraphicsEngine::get()->getRenderSystem();

	render_system->compileVertexShader(vertex_shader_name.c_str(), "vsmain", shader_byte_code, size_shader);
}

VertexShaderPtr ShaderLibrary::getVertexShader(String vertex_shader_name)
{
	if (m_active_vertex_shaders[vertex_shader_name] == NULL)
		std::cout << "Vertex shader not be found in list of active shaders" << std::endl;

	return m_active_vertex_shaders[vertex_shader_name];
}

PixelShaderPtr ShaderLibrary::getPixelShader(String pixel_shader_name)
{
	if (m_active_pixel_shaders[pixel_shader_name] == NULL)
		std::cout << "Pixel shader not be found in list of active shaders" << std::endl;

	return m_active_pixel_shaders[pixel_shader_name];
}