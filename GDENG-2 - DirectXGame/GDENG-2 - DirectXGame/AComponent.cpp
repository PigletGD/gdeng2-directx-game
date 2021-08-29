#include "AComponent.h"

AComponent::AComponent(String name, ComponentType type, AGameObject* owner) :
	m_name(name), m_type(type), m_owner(owner)
{
}

AComponent::~AComponent()
{
	m_owner = NULL;
	m_type = NotSet;
}

void AComponent::attachOwner(AGameObject* owner)
{
	m_owner = owner;
}

void AComponent::detachOwner()
{
	// component must also get deleted if object owner gets detatched
	m_owner = NULL;
	delete this;
}

AGameObject* AComponent::getOwner()
{
	return m_owner;
}

AComponent::ComponentType AComponent::getType()
{
	return m_type;
}

AComponent::String AComponent::getName()
{
	return m_name;
}

void AComponent::perform(float delta_time) {

}
