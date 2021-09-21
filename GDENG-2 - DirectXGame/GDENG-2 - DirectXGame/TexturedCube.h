#pragma once
#include "Cube.h"

class TexturedCube : public Cube
{
public:
	TexturedCube(String name, AGameObject::PrimitiveType type, std::wstring path);
	~TexturedCube();

	void draw(int width, int height) override;
	void updateTexture(TexturePtr newTex) override;
	TexturePtr getObjectTexture();

private:
	TexturePtr m_texture;
};