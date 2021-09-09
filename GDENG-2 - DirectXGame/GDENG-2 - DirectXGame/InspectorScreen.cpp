#include "InspectorScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GameObjectManager.h"
#include "UIManager.h"
#include "AGameObject.h"
#include "MathUtils.h"
#include "ActionHistory.h"

InspectorScreen::InspectorScreen() : AUIScreen("InspectorScreen")
{
}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::drawUI()
{
	ImGui::Begin("Inspector Window");
	m_selected_object = GameObjectManager::getInstance()->getSelectedObject();
	if (m_selected_object != NULL) {
		String name = m_selected_object->getName();
		ImGui::Text("Selected Object: %s", name.c_str());

		updateTransformText();
		bool enabled = m_selected_object->isEnabled();
		if (ImGui::Checkbox("Enabled", &enabled)) { m_selected_object->setEnabled(enabled); }
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			GameObjectManager::getInstance()->deleteObject(m_selected_object);
			GameObjectManager::getInstance()->setSelectedObject(NULL);
		}
		if (ImGui::InputFloat("Snap Speed", &m_snapping));
		if (ImGui::DragFloat3("Position", m_position_display, m_snapping)) { onTransformUpdate(); }
		if (ImGui::DragFloat3("Rotation", m_rotation_display, m_snapping)) { onTransformUpdate(); }
		if (ImGui::DragFloat3("Scale", m_scale_display, m_snapping)) { onTransformUpdate(); }
	}
	else {
		ImGui::Text("No object selected. Select an object first.");
	}
	ImGui::End();
}

void InspectorScreen::updateTransformText()
{
	Vector3D pos = m_selected_object->getLocalPosition();
	m_position_display[0] = pos.m_x;
	m_position_display[1] = pos.m_y;
	m_position_display[2] = pos.m_z;

	Vector3D rot = m_selected_object->getLocalRotation();
	m_rotation_display[0] = MathUtils::RadToDeg(rot.m_x);
	m_rotation_display[1] = MathUtils::RadToDeg(rot.m_y);
	m_rotation_display[2] = MathUtils::RadToDeg(rot.m_z);

	Vector3D scale = m_selected_object->getLocalScale();
	m_scale_display[0] = scale.m_x;
	m_scale_display[1] = scale.m_y;
	m_scale_display[2] = scale.m_z;
}

void InspectorScreen::onTransformUpdate()
{
	if (m_selected_object != NULL) {
		ActionHistory::getInstance()->recordAction(m_selected_object);

		m_selected_object->setPosition(Vector3D(m_position_display[0], m_position_display[1], m_position_display[2]));
		m_selected_object->setRotation(Vector3D(MathUtils::DegToRad(m_rotation_display[0]), MathUtils::DegToRad(m_rotation_display[1]), MathUtils::DegToRad(m_rotation_display[2])));
		m_selected_object->setScale(Vector3D(m_scale_display[0], m_scale_display[1], m_scale_display[2]));
	}
}