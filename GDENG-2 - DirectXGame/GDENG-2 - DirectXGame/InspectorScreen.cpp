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
	this->m_transformer_object = new TransformerObject("TransformerObject", true);
}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::drawUI()
{
	ImGui::Begin("Inspector Window");

	m_selected_objects = GameObjectManager::getInstance()->getSelectedObjectList();
	m_selected_object = GameObjectManager::getInstance()->getSelectedObject();

	if(m_selected_objects.size() > 1)//multiple selection mode
	{
		this->m_selected_object = this->m_transformer_object;
		int currentSize = m_selected_objects.size();

		if(this->m_selected_objects_size != currentSize)
		{
			//activate the TransformerObject or reset idk
		//	std::cout << "Activate TransformerObject\n";
			this->m_transformer_object->resetTransformations(m_selected_objects);
			this->m_selected_objects_size = currentSize;
			
		}
		
		ImGui::Text("Selected Objects: ");
		for(int n = 0 ; n < m_selected_objects.size() ; n++)
		{
			String objectName = m_selected_objects[n]->getName();
			ImGui::Text(objectName.c_str());
		}


		//start
		String name = m_selected_object->getName();
		ImGui::Text("Selected Object: %s", name.c_str());

		updateTransformText();
		
		if (ImGui::InputFloat("Snap Speed", &m_snapping));
		if (ImGui::DragFloat3("Position", m_position_display, m_snapping))
		{
			if (ImGui::IsItemEdited())
			{
				this->onPositionUpdate();
			}
			//onTransformUpdate();
		}
		if (ImGui::DragFloat3("Rotation", m_rotation_display, m_snapping))
		{
			if (ImGui::IsItemEdited())
			{
				this->onRotationUpdate();
			}
			//onTransformUpdate();
		}
		if (ImGui::DragFloat3("Scale", m_scale_display, m_snapping))
		{
			if (ImGui::IsItemEdited())
			{
				this->onScaleUpdate();
			}
			//onTransformUpdate();
		}



		
		ImGui::End();
		return;
	}

	
	if (m_selected_object != NULL) {//single selection mode
		//set current size to 0
		this->m_selected_objects_size = 0;
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
		if (ImGui::DragFloat3("Position", m_position_display, m_snapping))
		{
			if(ImGui::IsItemEdited())
			{
				this->onPositionUpdate();
			}
			//onTransformUpdate();
		}
		if (ImGui::DragFloat3("Rotation", m_rotation_display, m_snapping))
		{
			if(ImGui::IsItemEdited())
			{
				this->onRotationUpdate();
			}
			//onTransformUpdate();
		}
		if (ImGui::DragFloat3("Scale", m_scale_display, m_snapping))
		{
			if(ImGui::IsItemEdited())
			{
				this->onScaleUpdate();
			}
			//onTransformUpdate();
		}

		

		//for testing only!!! DELETE ME LATER BRUH
		ImGui::InputText("Add Child", str, IM_ARRAYSIZE(str));
		this->objectString = this->str;
		if(ImGui::IsItemEdited())
		{
			//this->objectString = stringBuffer.data();
			this->onAddObject(objectString);
		}
		
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

void InspectorScreen::onPositionUpdate()
{
	if (m_selected_object != NULL) {
		ActionHistory::getInstance()->recordAction(m_selected_object);
		m_selected_object->setPosition(Vector3D(m_position_display[0], m_position_display[1], m_position_display[2]));
		this->updateTransformText();
	}
}

void InspectorScreen::onRotationUpdate()
{
	if (m_selected_object != NULL) {
		ActionHistory::getInstance()->recordAction(m_selected_object);
		m_selected_object->setRotation(Vector3D(MathUtils::DegToRad(m_rotation_display[0]), MathUtils::DegToRad(m_rotation_display[1]), MathUtils::DegToRad(m_rotation_display[2])));
		this->updateTransformText();
	}
}

void InspectorScreen::onScaleUpdate()
{
	if (m_selected_object != NULL) {
		ActionHistory::getInstance()->recordAction(m_selected_object);
		m_selected_object->setScale(Vector3D(m_scale_display[0], m_scale_display[1], m_scale_display[2]));
		this->updateTransformText();
	}
}

void InspectorScreen::onAddObject(String objectName)
{
	if(m_selected_object != nullptr)
	{
		if(objectName.empty())
		{
			return;
		}

		AGameObject* object = GameObjectManager::getInstance()->findObjectByName(objectName);
		if(object == nullptr)
		{
			return;
		}
		m_selected_object->addChild(object);
		std::cout << "Added child object \n";
	}
	
}
