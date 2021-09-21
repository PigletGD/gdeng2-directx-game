#include "InspectorScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


#include "GameObjectManager.h"
#include "UIManager.h"
#include "AGameObject.h"
#include "MathUtils.h"
#include "ActionHistory.h"


#include "TexturedCube.h"
#include "Texture.h"
#include "TextureComponent.h"
#include "PhysicsComponent.h"
#include "GraphicsEngine.h"
#include <locale>
#include <codecvt>
#include <string>
#include <algorithm>


InspectorScreen::InspectorScreen() : AUIScreen("InspectorScreen")
{
	this->m_transformer_object = new TransformerObject("TransformerObject", true);
	m_file_browser = new ImGui::FileBrowser();
	m_file_browser->SetTitle("Change Texture");
	m_file_browser->SetTypeFilters({ ".png", ".jpg", ".jpeg" });
}

InspectorScreen::~InspectorScreen()
{
	delete m_file_browser;
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


		//components
		if (ImGui::TreeNode("Components"))
		{
			AGameObject::ComponentList compList = m_selected_object->getAllObjectComponents();
			for (int i = 0; i < compList.size(); i++)
			{
				// Use SetNextItemOpen() so set the default state of a node to be open. We could
				// also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!

				//texture component
				if(compList[i]->getType() == AComponent::Tex)
				{
					if(ImGui::TreeNode("Texture Component"))
					{
						//display component
						AGameObject::PrimitiveType t = m_selected_object->getObjectType();

						ID3D11ShaderResourceView* tptr = nullptr;
						TextureComponent* texComp = nullptr;
						//find the texture component
						for(int i = 0; i < compList.size(); i++)
						{
							if(compList[i]->getType() == AComponent::Tex)
							{
								texComp = dynamic_cast<TextureComponent*>(compList[i]);
							}
						}

						
						if(t == AGameObject::PrimitiveType::TEXTUREDCUBE)
						{
							TexturedCube* texCube = dynamic_cast<TexturedCube*>(m_selected_object);
							TexturePtr m_texture_to_display = texCube->getObjectTexture();
							tptr = m_texture_to_display->getTexture();
							
						}

						//display stuff
						ImGui::Image((void*)tptr, ImVec2(100, 100));
						
						if(ImGui::Button("Change Texture"))
						{
							this->m_file_browser->Open();
							//texComp->updateTexture() 
						}
						this->m_file_browser->Display();
						if(m_file_browser->HasSelected())
						{
							String path = m_file_browser->GetSelected().string();
							//open file
							String directory = path;
							std::cout << directory << std::endl;
							FileReader image_file;
							image_file.open(directory, std::ios::in);

		
							std::wstring path2(path.length(), L' ');//idk what the fuck
							std::copy(path.begin(), path.end(), path2.begin());

							
							//std::wstring wastring = (wastring)path;

							if (t == AGameObject::PrimitiveType::TEXTUREDCUBE)
							{
								texComp->updateTexture(GraphicsEngine::get()->getTextureManager()->createTextureFromFile(path2.c_str()));
								
							}
							
							//close
							image_file.close();
							m_file_browser->Close();
						}
						ImGui::TreePop();
					}
				}

				if(compList[i]->getType() == AComponent::Physics)
				{
					ImGui::Text("Physics Component Active");
				}
				/*
				if (i == 0)
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);

				if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
				{
					ImGui::Text("blah blah");
					ImGui::SameLine();
					if (ImGui::SmallButton("button")) {}
					ImGui::TreePop();
				}
				*/
			}
			ImGui::TreePop();
		}

		

		//for testing only!!! DELETE ME LATER BRUH
		ImGui::InputText("Add Child", str, IM_ARRAYSIZE(str));
		this->objectString = this->str;
		if(ImGui::IsItemEdited())
		{
			//this->objectString = stringBuffer.data();
			this->onAddObject(objectString);
		}

		if(ImGui::Button("Add Texture Component"))
		{
			this->onAddComponent(AComponent::Tex);
		}
		if (ImGui::Button("Add Physics Component"))
		{
			this->onAddComponent(AComponent::Physics);
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

void InspectorScreen::onAddComponent(AComponent::ComponentType componentType)
{
	AGameObject::ComponentList compList = {};
	if (m_selected_object == nullptr) return;
	if(m_selected_object != nullptr)
	{
		compList = m_selected_object->getComponentsOfType(componentType);
	}

	PhysicsComponent* physics_component = nullptr;
	TextureComponent* texture_component = nullptr;

	
	for(int i = 0 ; i < compList.size(); i++)
	{
		if(compList[i]->getType() == AComponent::Physics)
		{
			physics_component = dynamic_cast<PhysicsComponent*>(compList[i]);
		}
		if(compList[i]->getType() == AComponent::Tex)
		{
			texture_component = dynamic_cast<TextureComponent*>(compList[i]);
		}
	}
	

	if(componentType == AComponent::Tex && texture_component == nullptr)
	{
		TextureComponent* texture_component = new TextureComponent("TextureComponent", m_selected_object);
		m_selected_object->attachComponent(texture_component);
	}
	else if(componentType == AComponent::Physics && physics_component == nullptr)
	{
		//add physics component here brah
		PhysicsComponent* physics_component = new PhysicsComponent("PhysicsComponent", m_selected_object);
		m_selected_object->attachComponent(physics_component);
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
