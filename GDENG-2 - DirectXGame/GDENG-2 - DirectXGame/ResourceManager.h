#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include "Prerequisites.h"
#include "Resource.h"

class ResourceManager
{
public:
	typedef std::wstring String;
	typedef std::unordered_map<String, ResourcePtr> HashTable;

	ResourceManager();
	virtual ~ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* file_path);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;

private:
	HashTable m_map_resources;
};