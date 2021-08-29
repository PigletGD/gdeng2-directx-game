#pragma once

#include <string>

class AGameObject;

class AComponent
{
public:
	typedef std::string String;

	enum ComponentType { NotSet = -1, Script = 0, Renderer = 1, Input = 2, Physics = 3};

	AComponent(String name, ComponentType type, AGameObject* owner);
	~AComponent();

	void attachOwner(AGameObject* owner);
	void detachOwner();

	AGameObject* getOwner();
	ComponentType getType();
	String getName();

	virtual void perform(float delta_time) = 0;

protected:
	AGameObject* m_owner;
	ComponentType m_type;
	String m_name;
};