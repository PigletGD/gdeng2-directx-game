#include "ObjectHeirarchy.h"
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "InspectorScreen.h"
#include "UIManager.h"

ObjectHeirarchy::ObjectHeirarchy(String name) : AUIScreen(name), m_name(name)
{

}

ObjectHeirarchy::~ObjectHeirarchy()
{
}

void ObjectHeirarchy::drawUI()
{

	if (!m_is_open)
	{
		closeWindow(m_name);
		delete this;
		return;
	}


	ImGui::Begin("Scene Heirarchy");
	if (ImGui::TreeNode("Scene Heirarchy")) {

		static bool check = true;
		ImGui::Checkbox("active", &check);
		ImGui::SameLine();
		static int clicked = 0;
		if (ImGui::Button("Cube1")) {
			clicked++;
			this->onCreateInspectorScreen();
		}
			
		if (clicked & 1)
		{
			ImGui::SameLine();
			ImGui::Text("nice");
		}




		ImGui::TreePop();
	}
	ImGui::End();
}

void ObjectHeirarchy::onCreateInspectorScreen()
{
	UIManager* uiManager = UIManager::getInstance();
	UINames uiNames;

	if (uiManager->uiTable[uiNames.CREDITS_SCREEN] == nullptr) {
		InspectorScreen * inspector = new InspectorScreen(uiNames.INSPECTOR_SCREEN);
		uiManager->uiTable[uiNames.INSPECTOR_SCREEN] = inspector;
		uiManager->uiList.push_back(inspector);

		std::cout << "Created Inspector Screen" << std::endl;
	}
	else std::cout << "Inspector Screen Already Created" << std::endl;

}
