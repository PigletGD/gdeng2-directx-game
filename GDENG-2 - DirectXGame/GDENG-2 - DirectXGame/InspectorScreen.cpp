#include "InspectorScreen.h"
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "AppWindow.h"

InspectorScreen::InspectorScreen(String name) : AUIScreen(name), m_name(name)
{

}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::drawUI()
{

	if (!m_is_open)
	{
		closeWindow(m_name);
		delete this;
		return;
	}

	AppWindow* app = AppWindow::get();

	ImGui::Begin("Inspector Screen");
	if (ImGui::TreeNode("Transform")) {

		static float f1 = 1.00f, f2 = 0.0067f;
		static float arr[3] = {0,0,0};
		arr[0] = app->m_object_list[1]->getLocalPosition().m_x;
		arr[1] = app->m_object_list[1]->getLocalPosition().m_y;
		arr[2] = app->m_object_list[1]->getLocalPosition().m_z;
		ImGui::DragFloat3("Position",arr);

		arr[0] = app->m_object_list[1]->getLocalRotation().m_x;
		arr[1] = app->m_object_list[1]->getLocalRotation().m_y;
		arr[2] = app->m_object_list[1]->getLocalRotation().m_z;
		ImGui::DragFloat3("Rotation", arr);

		arr[0] = app->m_object_list[1]->getLocalScale().m_x;
		arr[1] = app->m_object_list[1]->getLocalScale().m_y;
		arr[2] = app->m_object_list[1]->getLocalScale().m_z;
		ImGui::DragFloat3("Scale", arr);

		//ImGui::DragFloat("drag float", &f1, 0.005f);
		//ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");

		/*
		static bool check = true;
		ImGui::Checkbox("active", &check);
		ImGui::SameLine();
		static int clicked = 0;
		if (ImGui::Button("Cube1"))
			clicked++;
		if (clicked & 1)
		{
			ImGui::SameLine();
			ImGui::Text("nice");
		}
		*/



		ImGui::TreePop();
	}
	ImGui::End();
}
