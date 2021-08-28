#pragma once
#include "Cube.h"

class TexturedCube : public Cube
{
public:
	TexturedCube(String name, std::wstring path);
	~TexturedCube();

	void draw(int width, int height) override;

private:
	TexturePtr m_texture;
};