#pragma once
#include "AGameObject.h"
#include "InputListener.h"

class Cube : public AGameObject, public InputListener
{
public:
	Cube(std::string name, void* shaderByteCode, size_t sizeShader);
	~Cube();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc) override;
	void setAnimSpeed(float speed);

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
private:
	VertexColorBufferPtr m_vcb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	float m_ticks = 0.0f;
	float m_delta_pos = 0.0f;
	float m_delta_time = 0.0f;
	float m_speed = 10.0f;
};

