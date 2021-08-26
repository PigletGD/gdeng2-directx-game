#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "Prerequisites.h"

//class VertexShader;
//class PixelShader;

class ShaderNames {
public:
	typedef std::wstring String;

	const String BASE_VERTEX_SHADER_NAME = L"VertexShader.hlsl";
	const String LERP_VERTEX_SHADER_NAME = L"LerpVertexShader.hlsl";
	const String TEXTURED_VERTEX_SHADER_NAME = L"TexturedVertexShader.hlsl";

	const String BASE_PIXEL_SHADER_NAME = L"PixelShader.hlsl";
	const String LERP_PIXEL_SHADER_NAME = L"LerpPixelShader.hlsl";
	const String TEXTURED_PIXEL_SHADER_NAME = L"TexturedPixelShader.hlsl";
};

class ShaderLibrary
{
public:
	class ShaderData {
	public:
		void* shader_byte_code = NULL;
		size_t size_shader = 0;
	};

	typedef std::wstring String;
	typedef std::unordered_map<String, VertexShaderPtr> VertexShaderTable;
	typedef std::unordered_map<String, PixelShaderPtr> PixelShaderTable;

	static ShaderLibrary* getInstance();
	static void initialize();
	static void destroy();

	void requestVertexShaderData(String vertex_shader_name, void** shader_byte_code, size_t* size_shader);
	VertexShaderPtr getVertexShader(String vertex_shader_name);
	PixelShaderPtr getPixelShader(String pixel_shader_name);

private:
	ShaderLibrary();
	~ShaderLibrary();
	ShaderLibrary(ShaderLibrary const&) {};
	ShaderLibrary& operator=(ShaderLibrary const&) {};
	static ShaderLibrary* m_shared_instance;

	VertexShaderTable m_active_vertex_shaders;
	PixelShaderTable m_active_pixel_shaders;
};