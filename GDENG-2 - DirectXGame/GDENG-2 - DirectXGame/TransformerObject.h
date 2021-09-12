#pragma once
#include "AGameObject.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
class TransformerObject :
    public AGameObject
{
public:
	TransformerObject(std::string name, bool skip_init = false);
	~TransformerObject();

	void update(float delta_time) override;
	virtual void draw(int width, int height) override;

	typedef std::vector<AGameObject*> SelectedObjectList;
	typedef std::vector<AGameObject*> ChildList;
	void resetTransformations(SelectedObjectList objectList);
	Vector3D getMidpointFromObjectList(SelectedObjectList objectList);

protected:
	//ChildList m_child_list;
};

