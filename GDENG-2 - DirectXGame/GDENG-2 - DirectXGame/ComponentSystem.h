#pragma once

#include <string>

class PhysicsSystem;

class ComponentSystem
{
public:
	typedef std::string String;

	static ComponentSystem* getInstance();
	static void initialize();
	static void destroy();

	PhysicsSystem* getPhysicsSystem();

private:
	ComponentSystem();
	~ComponentSystem();
	ComponentSystem(ComponentSystem const&) {};             // copy constructor is private
	ComponentSystem& operator=(ComponentSystem const&) {};  // assignment operator is private*/
	static ComponentSystem* shared_instance;

	PhysicsSystem* m_physics_system;
};