#include "TransformerObject.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "Vertex.h"
#include "ConstantData.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "MathUtils.h"
#include "ShaderLibrary.h"

TransformerObject::TransformerObject(std::string name, bool skip_init) :
	AGameObject(name)
{
	if (skip_init) return;
}

TransformerObject::~TransformerObject()
{
	AGameObject::~AGameObject();
}

void TransformerObject::update(float delta_time)
{

}

void TransformerObject::draw(int width, int height)
{
	
}

void TransformerObject::resetTransformations(SelectedObjectList objectList)
{
	//clear all the child list
	this->m_child_list.clear();

	
	//reset rotation
	AQuaternion rot = {};
	rot.m_x = 0;
	rot.m_y = 0;
	rot.m_z = 0;
	rot.m_w = 1;
	//this->setRotation(rot.m_x, rot.m_y, rot.m_z);
	this->m_orientation = rot;
	
	
	//reset scale
	//this->m_local_scale.one();
	this->setScale(Vector3D::one());
	
	//reset position to midpoint
	Vector3D newPosition = {};
	newPosition = this->getMidpointFromObjectList(objectList);
	this->setPosition(newPosition);

	//populate with children
	for(int i = 0 ; i<objectList.size(); i++)
	{
		this->addChild(objectList[i]);
	}
}

Vector3D TransformerObject::getMidpointFromObjectList(SelectedObjectList objectList)
{
	Vector3D newPosition = {};
	
	for(int i = 0; i < objectList.size() ; i++)
	{
		newPosition.m_x += objectList[i]->getLocalPosition().m_x;
		newPosition.m_y += objectList[i]->getLocalPosition().m_y;
		newPosition.m_z += objectList[i]->getLocalPosition().m_z;
	}
	const float dividend = objectList.size();
	newPosition.m_x /= dividend;
	newPosition.m_y /= dividend;
	newPosition.m_z /= dividend;

	return newPosition;
}
