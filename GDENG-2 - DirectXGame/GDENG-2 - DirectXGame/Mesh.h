#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "AGameObject.h"
#include "InputListener.h"


class Mesh : public Resource, public AGameObject, public InputListener
{
public:
	Mesh(const wchar_t* full_path);
	~Mesh();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShaderPtr vertexShader, PixelShaderPtr pixelShader, constant cc) override;

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	ConstantBufferPtr m_cb;

	float m_ticks = 0.0f;
	float m_delta_pos = 0.0f;
	float m_delta_time = 0.0f;
	float m_speed = 10.0f;

	friend class DeviceContext;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
};