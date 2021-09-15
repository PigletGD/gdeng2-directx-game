#include "SceneReader.h"

#include <iostream>
#include <fstream>

#include "GameObjectManager.h"
#include "StringUtils.h"

typedef std::fstream FileReader;

SceneReader::SceneReader(String directory)
{
	m_directory = directory;
}

SceneReader::~SceneReader()
{
}

void SceneReader::readFromFile()
{
	String file_dir = m_directory + ".level";
	if (m_directory.find(".level") != String::npos) {
		file_dir = m_directory;
	}

	FileReader scene_file;
	scene_file.open(file_dir, std::ios::in);

	int index = 0;
	String read_line;

	String object_name;
	AGameObject::PrimitiveType object_type;
	Vector3D position;
	Vector3D rotation;
	Vector3D scale;

	while (std::getline(scene_file, read_line)) {
		if (index == 0) {
			object_name = read_line;
			index++;
		}
		else if (index == 1) {
			std::vector string_split = StringUtils::split(read_line, ' ');
			object_type = (AGameObject::PrimitiveType)std::stoi(string_split[1]);
			index++;
		}
		else if (index == 2) {
			std::vector string_split = StringUtils::split(read_line, ' ');
			position = Vector3D(std::stof(string_split[1]), std::stof(string_split[2]), std::stof(string_split[3]));
			index++;
		}
		else if (index == 3) {
			std::vector string_split = StringUtils::split(read_line, ' ');
			rotation = Vector3D(std::stof(string_split[1]), std::stof(string_split[2]), std::stof(string_split[3]));
			index++;
		}
		else if (index == 4) {
			std::vector string_split = StringUtils::split(read_line, ' ');
			scale = Vector3D(std::stof(string_split[1]), std::stof(string_split[2]), std::stof(string_split[3]));
			index = 0;

			GameObjectManager::getInstance()->createObjectFromFile(object_name, object_type, position, rotation, scale);
		}
	}
}