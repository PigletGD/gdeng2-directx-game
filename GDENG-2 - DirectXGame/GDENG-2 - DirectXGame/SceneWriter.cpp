#include "SceneWriter.h"

#include <iostream>
#include <fstream>

#include "GameObjectManager.h"

typedef std::fstream FileWriter;

SceneWriter::SceneWriter(String directory)
{
	m_directory = directory;
}

SceneWriter::~SceneWriter()
{
}

void SceneWriter::writeToFile()
{
	String file_dir = m_directory + ".level";
	if (m_directory.find(".level") != String::npos)
		file_dir = m_directory;

	FileWriter scene_file;
	scene_file.open(file_dir, std::ios::out);

	std::cout << "Selected filename " << file_dir << "\n";

	GameObjectManager::List allObjects = GameObjectManager::getInstance()->getAllObjects();

	for (int i = 0; i < allObjects.size(); i++) {
		scene_file << allObjects[i]->getName() << std::endl;
		Vector3D position = allObjects[i]->getLocalPosition();
		Vector3D rotation = allObjects[i]->getLocalRotation();
		Vector3D scale = allObjects[i]->getLocalScale();

		scene_file << "Type: " << allObjects[i]->getObjectType() << std::endl;
		scene_file << "Position: " << position.getX() << " " << position.getY() << " " << position.getZ() << std::endl;
		scene_file << "Rotation: " << rotation.getX() << " " << rotation.getY() << " " << rotation.getZ() << std::endl;
		scene_file << "Scale: " << scale.getX() << " " << scale.getY() << " " << scale.getZ() << std::endl;
	}

	scene_file.close();
}