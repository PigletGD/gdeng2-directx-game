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
	typedef std::vector<AGameObject*> SelectedObjectList;
	typedef std::unordered_map<String, AGameObject*> HashTable;

	static GameObjectManager* getInstance();
	static void initialize();
	static void destroy();

	AGameObject* findObjectByName(String name);
	List getAllObjects();

	int activeObjects();

	void updateAll();
	void renderAll(int viewport_width, int viewport_height);

	void addObject(AGameObject* game_object);
	void createObject(AGameObject::PrimitiveType type);
	void createObject(std::wstring mesh_path, std::wstring texture_path = L"");
	void createObjectFromFile(String name, AGameObject::PrimitiveType type, Vector3D position, Vector3D rotation, Vector3D scale);

	void deleteObject(AGameObject* game_object);
	void deleteObjectByName(String name);

	void setSelectedObject(String name);
	void setSelectedObject(AGameObject* game_object);
	void addSelectedObjectToSelectedList(String name);
	void removeSelectedObjectToSelectedList(String name);
	void clearSelectedObject();
	AGameObject* getSelectedObject();
	SelectedObjectList getSelectedObjectList();

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
	SelectedObjectList m_selected_objects;
	
	AGameObject* m_selected_object = NULL;
};