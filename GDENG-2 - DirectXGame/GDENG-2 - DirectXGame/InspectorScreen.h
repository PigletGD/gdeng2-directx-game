#pragma once
#include "AUIScreen.h"

class AGameObject;

class InspectorScreen : public AUIScreen
{
private:
	InspectorScreen();
	~InspectorScreen();

	virtual void drawUI() override;
	
	void updateTransformText();
	void onTransformUpdate();

	float m_snapping = 0.1f;

	float m_position_display[3] = { 0.0f, 0.0f, 0.0f };
	float m_rotation_display[3] = { 0.0f, 0.0f, 0.0f };
	float m_scale_display[3] = { 1.0f, 1.0f, 1.0f };

	AGameObject* m_selected_object = NULL;

	friend class UIManager;
};