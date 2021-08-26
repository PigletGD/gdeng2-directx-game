#pragma once
#include "Cube.h"
class TexturedCube : public Cube
{
public:
	TexturedCube(String name);
	~TexturedCube();

	void draw(int width, int height) override;
};