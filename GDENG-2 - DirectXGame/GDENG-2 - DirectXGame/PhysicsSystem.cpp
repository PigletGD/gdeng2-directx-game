#include "PhysicsSystem.h"

#include <iostream>

#include "PhysicsComponent.h"
#include "EngineTime.h"

PhysicsSystem::PhysicsSystem()
{
	// Create the physics engine and world
	m_physics_common = new PhysicsCommon();

	PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 50;
	settings.gravity = Vector3(0, -9.81f, 0);
	
	m_physics_world = m_physics_common->createPhysicsWorld(settings);

	std::cout << "Successfully created physics world" << std::endl;
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::registerComponent(PhysicsComponent* component)
{
	m_component_table[component->getName()] = component;
	m_component_list.push_back(component);
}

void PhysicsSystem::unregisterComponent(PhysicsComponent* component)
{
	if (m_component_table[component->getName()] != NULL) {
		m_component_table.erase(component->getName());

		int index = -1;
		for (int i = 0; i < m_component_list.size(); i++) {
			if (m_component_list[i] == component) {
				index = i;
				break;
			}
		}

		if (index != -1) m_component_list.erase(m_component_list.begin() + index);
	}
	else std::cout << "Component " << component->getName() << " not registered in physics component" << std::endl;
}

void PhysicsSystem::unregisterComponentByName(String name)
{
	if (m_component_table[name] != NULL)
		unregisterComponent(m_component_table[name]);
}

PhysicsComponent* PhysicsSystem::findComponentByName(String name)
{
	if (m_component_table[name] == NULL)
		std::cout << "Component " << name << " not registered in physics component" << std::endl;

	return m_component_table[name];
}

PhysicsSystem::ComponentList PhysicsSystem::getAllComponents()
{
	return m_component_list;
}

void PhysicsSystem::updateAllComponents()
{
	// skip first frame
	if (EngineTime::getDeltaTime() > 0.0f) {

		for (int i = 0; i < m_component_list.size(); i++) {
			m_component_list[i]->preperform();
		}

		//update physics world
		m_physics_world->update(EngineTime::getDeltaTime());

		for (int i = 0; i < m_component_list.size(); i++) {
			m_component_list[i]->perform(EngineTime::getDeltaTime());
		}
	}
}

PhysicsWorld* PhysicsSystem::getPhysicsWorld()
{
    return m_physics_world;
}

PhysicsCommon* PhysicsSystem::getPhysicsCommon()
{
    return m_physics_common;
}
