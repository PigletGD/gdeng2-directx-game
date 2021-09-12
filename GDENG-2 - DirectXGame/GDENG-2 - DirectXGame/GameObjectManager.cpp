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
        String revisedString = game_object->getName() + " " + "(" + std::to_string(count) + ")";
        while (m_game_object_map[revisedString] != NULL) {
            count++;
            revisedString = game_object->getName() + " " + "(" + std::to_string(count) + ")";
        }
        game_object->setName(revisedString);
        m_game_object_map[revisedString] = game_object;
    }
    else m_game_object_map[game_object->getName()] = game_object;

    m_game_object_list.push_back(game_object);
}

void GameObjectManager::createObject(PrimitiveType type)
{
    if (type == PrimitiveType::CUBE) {
        Cube* cube = new Cube("Cube");
        cube->updateLocalMatrix();
        addObject(cube);
    }

    else if (type == PrimitiveType::PHYSICS_CUBE) {
        PhysicsCube* cube = new PhysicsCube("Physics Cube");
        addObject(cube);
    }

    else if (type == PrimitiveType::TEXTURED_CUBE) {
        TexturedCube* cube = new TexturedCube("Cube Textured", L"Assets\\Textures\\wood.jpg");
        cube->updateLocalMatrix();
        addObject(cube);
    }

    else if (type == PrimitiveType::PHYSICS_CUBE_BATCH) {
        for (int i = 0; i < 20; i++) {
            PhysicsCube* cube = new PhysicsCube("Physics Cube");
            addObject(cube);
        }
    }

    else if (type == PrimitiveType::PLANE) {
        Plane* plane = new Plane("Plane");
        //plane->setRotation(MathUtils::DegToRad(90), 0.0f, 0.0f);
        plane->setScale(7.0f, 0.2f, 7.0f);
        plane->updateLocalMatrix();
        addObject(plane);
    }

    else if (type == PrimitiveType::PHYSICS_PLANE) {
        PhysicsPlane* plane = new PhysicsPlane("Plane");
        addObject(plane);
    }
}

void GameObjectManager::createObject(std::wstring mesh_path, std::wstring texture_path)
{
    MeshObject* mesh = new MeshObject("Mesh", mesh_path, texture_path);
    mesh->updateLocalMatrix();
    addObject(mesh);
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
