#include "GameObjectManager.h"
#include "EngineTime.h"
#include "Cube.h"
#include "Plane.h"
#include "MathUtils.h"
#include "AGameObject.h"
#include "TexturedCube.h"
#include "ShaderLibrary.h"
#include "GraphicsEngine.h"

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
        cube->setPosition(0.0f, 0.0f, 0.0f);
        cube->setScale(1.0f, 1.0f, 1.0f);
        this->addObject(cube);
    }

    else if (type == PrimitiveType::PLANE) {
        Plane* plane = new Plane("Plane");
        plane->setRotation(MathUtils::DegToRad(90), 0.0f, 0.0f);
        plane->setScale(7.0f, 7.0f, 7.0f);
        this->addObject(plane);
    }

    else if (type == PrimitiveType::TEXTURED_CUBE) {
        TexturedCube* cube = new TexturedCube("Cube_Textured");
        cube->setPosition(2.0f, 2.0f, 2.0f);
        cube->setScale(1.0f, 1.0f, 1.0f);
        this->addObject(cube);
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

AGameObject* GameObjectManager::getSelectedObject()
{
    return m_selected_object;
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}
