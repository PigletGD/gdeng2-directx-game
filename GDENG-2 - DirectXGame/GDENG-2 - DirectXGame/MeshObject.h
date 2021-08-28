#pragma once
#include "AGameObject.h"

class MeshObject : public AGameObject
{
public:
	MeshObject(std::string name, std::wstring mesh_path, std::wstring texture_path = L"", bool skip_init = false);
	~MeshObject();

	void update(float delta_time) override;
	void draw(int width, int height) override;

private:
	MeshPtr m_mesh;
	TexturePtr m_texture;
	ConstantBufferPtr m_cb;
};