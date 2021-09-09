#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include "AGameObject.h"

class EditorAction;

class GameObjectManager
{
public:
	typedef std::string String;
	typedef std::vector<AGameObject*> List;
	typedef std::unordered_map<String, AGameObject*> HashTable;

	enum PrimitiveType {
		CUBE,
		PHYSICS_CUBE,
		PHYSICS_CUBE_BATCH,
		TEXTURED_CUBE,
		PLANE,
		PHYSICS_PLANE,
	};

	static GameObjectManager* getInstance();
	static void initialize();
	static void destroy();

	AGameObject* findObjectByName(String name);
	List getAllObjects();

	int activeObjects();

	void updateAll();
	void renderAll(int viewport_width, int viewport_height);

	void addObject(AGameObject* game_object);
	void createObject(PrimitiveType type);
	void createObject(std::wstring mesh_path, std::wstring texture_path = L"");

	void deleteObject(AGameObject* game_object);
	void deleteObjectByName(String name);

	void setSelectedObject(String name);
	void setSelectedObject(AGameObject* game_object);
	AGameObject* getSelectedObject();

	void saveEditStates();
	void restoreEditStates();
	void applyEditorAction(EditorAction* action);

private:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(GameObjectManager const&) {};             // copy constructor is private
	GameObjectManager& operator=(GameObjectManager const&) {};  // assignment operator is private*/
	static GameObjectManager* m_shared_instance;

	HashTable m_game_object_map;
	List m_game_object_list;

	AGameObject* m_selected_object = NULL;
};