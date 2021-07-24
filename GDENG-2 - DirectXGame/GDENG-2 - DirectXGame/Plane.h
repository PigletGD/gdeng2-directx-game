#pragma once
#include "AGameObject.h"

#include "ConstantData.h"

class Plane : public AGameObject
{
public:
	Plane(std::string name, void* shaderByteCode, size_t sizeShader);
	~Plane();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc) override;

private:
	VertexColorBufferPtr m_vcb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
};