#pragma once
#include <string>

class Resource
{
public:
	typedef std::wstring String;

	Resource(const wchar_t* full_path);
	virtual ~Resource();

protected:
	String m_full_path;
};
