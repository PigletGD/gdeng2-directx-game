#include "Texture.h"
#include "GraphicsEngine.h"

#include <DirectXTex.h>

Texture::Texture(const wchar_t* full_path) : 
	Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, NULL, image_data);

	if (FAILED(res)) throw std::exception("Texture not created successfully");

	ID3D11Device* device = GraphicsEngine::get()->getRenderSystem()->getDevice();

	res = DirectX::CreateTexture(device, image_data.GetImages(),
		image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = (UINT)image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(m_texture, &desc, &m_shader_res_view);
}

Texture::~Texture()
{
	Resource::~Resource();
	m_shader_res_view->Release();
	m_texture->Release();
}

Resource::String Texture::getPath()
{
	return m_full_path;
}

ID3D11ShaderResourceView* Texture::getTexture()
{
	return m_shader_res_view;
}
