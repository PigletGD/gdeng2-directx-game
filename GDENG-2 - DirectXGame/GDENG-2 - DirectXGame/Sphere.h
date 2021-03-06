#pragma once
#include "AGameObject.h"
#include "PhysicsComponent.h"


class Sphere : public AGameObject
{
public:
	Sphere(std::string name, AGameObject::PrimitiveType type);
	~Sphere();

	void update(float delta_time) override;
	void draw(int width, int height) override;

	void saveEditState() override;
	void restoreEditState() override;

	float getRadius();

	float radius = 1;
protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	
	PhysicsComponent* m_component_attached;

	float m_ticks = 0.0f;
};
