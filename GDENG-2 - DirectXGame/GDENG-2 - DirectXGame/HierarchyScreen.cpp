#include "HierarchyScreen.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GameObjectManager.h"
#include "UIManager.h"

#include <iostream>

HierarchyScreen::HierarchyScreen() :
	AUIScreen("HierarchyScreen")
{
}

HierarchyScreen::~HierarchyScreen()
{
}

void HierarchyScreen::drawUI()
{
	ImGui::Begin("Scene Outliner");
	//ImGui::SetWindowPos(ImVec2(UIManager::WINDOW_WIDTH - 525, 20));
	//ImGui::SetWindowSize(ImVec2(250, UIManager::WINDOW_HEIGHT));

	updateObjectList();

	ImGui::End();
	
}

void HierarchyScreen::updateObjectList() const
{
	GameObjectManager::List objectList = GameObjectManager::getInstance()->getAllObjects();


    if (ImGui::TreeNode("Scene Objects"))
    {
        //static bool selection[objectList.size()] = { false };
        for (int n = 0; n < objectList.size(); n++)
        {
            String objectName = objectList[n]->getName();
            if (ImGui::Selectable(objectName.c_str(), objectList[n]->m_selected))
            {
                if (!ImGui::GetIO().KeyCtrl){    // Clear selection when CTRL is not held
                    for (AGameObject* object : objectList)
                    {
                        object->m_selected = false;
                    }
                    GameObjectManager::getInstance()->clearSelectedObject();
                    //GameObjectManager::getInstance()->setSelectedObject(objectName);//1 onli
                   
				}
            	if(objectList[n]->m_selected == false)
            	{
                    objectList[n]->m_selected ^= 1;
                    GameObjectManager::getInstance()->addSelectedObjectToSelectedList(objectName);
                    GameObjectManager::getInstance()->setSelectedObject(objectName);//1 onli
            	}
                
            
            }
        }
        ImGui::TreePop();
    }
	

	/*for (int i = 0; i < objectList.size(); i++) {
		String objectName = objectList[i]->getName();
		if (ImGui::Button(objectName.c_str(), ImVec2(235, 0))) {
			GameObjectManager::getInstance()->setSelectedObject(objectName);
		}
	}*/
}
