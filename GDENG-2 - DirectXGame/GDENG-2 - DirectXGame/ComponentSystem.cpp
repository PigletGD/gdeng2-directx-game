#include "ComponentSystem.h"
#include "PhysicsSystem.h"

ComponentSystem* ComponentSystem::shared_instance = NULL;
ComponentSystem* ComponentSystem::getInstance()
{
    return shared_instance;
}

void ComponentSystem::initialize()
{
	shared_instance = new ComponentSystem();
}

void ComponentSystem::destroy()
{
	delete shared_instance;
}

PhysicsSystem* ComponentSystem::getPhysicsSystem()
{
    return m_physics_system;
}

ComponentSystem::ComponentSystem()
{
	m_physics_system = new PhysicsSystem();
}

ComponentSystem::~ComponentSystem()
{
	delete m_physics_system;
}