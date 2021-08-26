#pragma once

#include "Resource.h"
#include "CreditsScreen.h"

#include <d3d11.h>

class Texture : public Resource
{
public:
	Texture(const wchar_t* full_path);
	~Texture();

	Resource::String getPath();
	ID3D11ShaderResourceView* getTexture();

private:
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_res_view = nullptr;

	friend class DeviceContext;
};