#pragma once
#include "AUIScreen.h"
#include <iostream>
#include <string>
#include <vector>
#include "TransformerObject.h"
class AGameObject;

class InspectorScreen : public AUIScreen
{
private:
	InspectorScreen();
	~InspectorScreen();

	typedef std::string String;
	typedef std::vector<AGameObject*> SelectedObjectList;

	virtual void drawUI() override;

	void updateTransformText();
	void onTransformUpdate();
	void onPositionUpdate();
	void onRotationUpdate();
	void onScaleUpdate();
	
	std::vector<char> stringBuffer = {'q'};
	char str[128] = {};
	
	std::string objectString;
	void onAddObject(String objectName);
	
	float m_snapping = 0.1f;

	float m_position_display[3] = { 0.0f, 0.0f, 0.0f };
	float m_rotation_display[3] = { 0.0f, 0.0f, 0.0f };
	float m_scale_display[3] = { 1.0f, 1.0f, 1.0f };

	int m_selected_objects_size = 0;
	
	AGameObject* m_selected_object = NULL;
	SelectedObjectList m_selected_objects;

	TransformerObject* m_transformer_object = nullptr;
	friend class UIManager;
};