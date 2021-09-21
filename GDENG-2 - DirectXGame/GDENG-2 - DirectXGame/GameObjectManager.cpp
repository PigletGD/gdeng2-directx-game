#include "GameObjectManager.h"
#include "EngineTime.h"
#include "Cube.h"
#include "Plane.h"
#include "MathUtils.h"
#include "AGameObject.h"
#include "TexturedCube.h"
#include "MeshObject.h"
#include "ShaderLibrary.h"
#include "GraphicsEngine.h"
#include "PhysicsCube.h"
#include "PhysicsPlane.h"
#include "EditorAction.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Capsule.h"

GameObjectManager* GameObjectManager::m_shared_instance = NULL;

GameObjectManager* GameObjectManager::getInstance()
{
    return m_shared_instance;
}

void GameObjectManager::initialize()
{
    m_shared_instance = new GameObjectManager();
}

void GameObjectManager::destroy()
{
    m_shared_instance->m_game_object_map.clear();
    m_shared_instance->m_game_object_list.clear();
    delete m_shared_instance;
}

AGameObject* GameObjectManager::findObjectByName(String name)
{
    if (m_game_object_map[name] == NULL)
        std::cout << "Object " << name << " not found!";

    return m_game_object_map[name];
}

GameObjectManager::List GameObjectManager::getAllObjects()
{
    return m_game_object_list;
}

int GameObjectManager::activeObjects()
{
    return m_game_object_list.size();
}

void GameObjectManager::updateAll()
{
    for (int i = 0; i < m_game_object_list.size(); i++)
        if (m_game_object_list[i]->isEnabled())
            m_game_object_list[i]->update(EngineTime::getDeltaTime());
}

void GameObjectManager::renderAll(int viewport_width, int viewport_height)
{
    for (int i = 0; i < m_game_object_list.size(); i++)
        if (m_game_object_list[i]->isEnabled())
            m_game_object_list[i]->draw(viewport_width, viewport_height);
}

void GameObjectManager::addObject(AGameObject* game_object)
{
    if (m_game_object_map[game_object->getName()] != NULL) {
        int count = 1;
        String revisedString = game_object->getName() + "(" + std::to_string(count) + ")";
        while (m_game_object_map[revisedString] != NULL) {
            count++;
            revisedString = game_object->getName() + "(" + std::to_string(count) + ")";
        }
        game_object->setName(revisedString);
        m_game_object_map[revisedString] = game_object;
    }
    else m_game_object_map[game_object->getName()] = game_object;

    m_game_object_list.push_back(game_object);
}

void GameObjectManager::createObject(AGameObject::PrimitiveType type)
{
    if (type == AGameObject::PrimitiveType::CUBE) {
        Cube* cube = new Cube("Cube", type);
        cube->updateLocalMatrix();
        addObject(cube);
    }

    else if (type == AGameObject::PrimitiveType::PHYSICSCUBE) {
        PhysicsCube* cube = new PhysicsCube("PhysicsCube", type);
        addObject(cube);
    }

    else if (type == AGameObject::PrimitiveType::TEXTUREDCUBE) {
        TexturedCube* cube = new TexturedCube("CubeTextured", type, L"Assets\\Textures\\White.png");
        cube->updateLocalMatrix();
        addObject(cube);
    }

    else if (type == AGameObject::PrimitiveType::PHYSICSCUBEBATCH) {
        for (int i = 0; i < 20; i++) {
            PhysicsCube* cube = new PhysicsCube("PhysicsCube", type);
            addObject(cube);
        }
    }

    else if (type == AGameObject::PrimitiveType::PLANE) {
        Plane* plane = new Plane("Plane", type);
        plane->setScale(7.0f, 0.2f, 7.0f);
        plane->updateLocalMatrix();
        addObject(plane);
    }
    else if (type == AGameObject::PrimitiveType::PHYSICSPLANE) {
        PhysicsPlane* plane = new PhysicsPlane("Plane", type);
        addObject(plane);
    }

    else if (type == AGameObject::PrimitiveType::SPHERE) {
        Sphere* sphere = new Sphere("Sphere", type);
        addObject(sphere);
    }

    else if (type == AGameObject::PrimitiveType::CYLINDER) {
        Cylinder* cylinder = new Cylinder("Cylinder", type);
        addObject(cylinder);
    }
    else if (type == AGameObject::PrimitiveType::CAPSULE) {
        Capsule* capsule = new Capsule("Capsule", type);
        addObject(capsule);
    }
	
}

void GameObjectManager::createObject(std::wstring mesh_path, std::wstring texture_path)
{
    MeshObject* mesh = new MeshObject("Mesh", AGameObject::PrimitiveType::MESH, mesh_path, texture_path);
    mesh->updateLocalMatrix();
    addObject(mesh);
}

void GameObjectManager::createObjectFromFile(String name, AGameObject::PrimitiveType type, Vector3D position, Vector3D rotation, Vector3D scale)
{
    if (type == AGameObject::PrimitiveType::CUBE) {
        Cube* cube = new Cube(name, type);
        cube->setPosition(position);
        cube->setRotation(rotation);
        cube->setScale(scale);
        addObject(cube);
    }

    else if (type == AGameObject::PrimitiveType::PLANE) {
        Plane* plane = new Plane(name, type);
        plane->setPosition(position);
        plane->setRotation(rotation);
        plane->setScale(scale);
        addObject(plane);
    }

    else if (type == AGameObject::PrimitiveType::TEXTUREDCUBE) {
        TexturedCube* cube = new TexturedCube(name, type, L"Assets\\Textures\\wood.jpg");
        cube->setPosition(position);
        cube->setRotation(rotation);
        cube->setScale(scale);
        addObject(cube);
    }

    else if (type == AGameObject::PrimitiveType::PHYSICSCUBE) {
        PhysicsCube* cube = new PhysicsCube(name, type);
        cube->setPosition(position);
        cube->setRotation(rotation);
        cube->setScale(scale);
        addObject(cube);
    }

    else if (type == AGameObject::PrimitiveType::PHYSICSPLANE) {
        PhysicsPlane* plane = new PhysicsPlane(name, type);
        plane->setPosition(position);
        plane->setRotation(rotation);
        plane->setScale(scale);
        addObject(plane);
    }

    else if (type == AGameObject::PrimitiveType::SPHERE) {
        Sphere* sphere = new Sphere(name, type);
        sphere->setPosition(position);
        sphere->setRotation(rotation);
        sphere->setScale(scale);
        addObject(sphere);
    }

    else if (type == AGameObject::PrimitiveType::CYLINDER) {
        Cylinder* cylinder = new Cylinder(name, type);
        cylinder->setPosition(position);
        cylinder->setRotation(rotation);
        cylinder->setScale(scale);
        addObject(cylinder);
    }
}

void GameObjectManager::deleteObject(AGameObject* game_object)
{
    m_game_object_map.erase(game_object->getName());

    int index = -1;
    for (int i = 0; i < m_game_object_list.size(); i++) {
        if (m_game_object_list[i] == game_object) {
            index = i;
            break;
        }
    }

    if (index != -1) m_game_object_list.erase(m_game_object_list.begin() + index);

    delete game_object;
}

void GameObjectManager::deleteObjectByName(String name)
{
    AGameObject* object = findObjectByName(name);

    if (object != NULL) deleteObject(object);
}

void GameObjectManager::setSelectedObject(String name)
{
    if (m_game_object_map[name] != NULL)
        setSelectedObject(m_game_object_map[name]);
}

void GameObjectManager::setSelectedObject(AGameObject* game_object)
{
    m_selected_object = game_object;
}

void GameObjectManager::addSelectedObjectToSelectedList(String name)
{
    if (m_game_object_map[name] != NULL)
        this->m_selected_objects.push_back(m_game_object_map[name]);
}

void GameObjectManager::removeSelectedObjectToSelectedList(String name)
{
    int index = -1;
    if (m_game_object_map[name] != NULL)
    {
        
        AGameObject* store = m_game_object_map[name];
    	for(int i = 0 ; i<this->m_selected_objects.size(); i++)
    	{
    		if(store == m_selected_objects[i])
    		{
                index = i;
                break;
    		}
    	}
    }
    if(index != -1)
	  m_selected_objects.erase(m_selected_objects.begin() + index);
}

void GameObjectManager::clearSelectedObject()
{
    m_selected_objects.clear();
}

AGameObject* GameObjectManager::getSelectedObject()
{
    return m_selected_object;
}

GameObjectManager::SelectedObjectList GameObjectManager::getSelectedObjectList()
{
    return m_selected_objects;
}

void GameObjectManager::saveEditStates()
{
    for (int i = 0; i < m_game_object_list.size(); i++)
        m_game_object_list[i]->saveEditState();
}

void GameObjectManager::restoreEditStates()
{
    for (int i = 0; i < m_game_object_list.size(); i++)
        m_game_object_list[i]->restoreEditState();
}

void GameObjectManager::applyEditorAction(EditorAction* action)
{
    AGameObject* object = findObjectByName(action->getOwnerName());

    if (object == NULL) return;

    object->setLocalMatrix(action->getStoredMatrix().getMatrix());
    object->setPosition(action->getStorePos());
    object->setRotation(action->getStoredOrientation().m_x, action->getStoredOrientation().m_y, action->getStoredOrientation().m_z);
    object->setScale(action->getStoredScale());
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}
