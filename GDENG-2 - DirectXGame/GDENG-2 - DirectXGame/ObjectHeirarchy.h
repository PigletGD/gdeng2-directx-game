#pragma once
#include "AUIScreen.h"
class ObjectHeirarchy :
    public AUIScreen
{
public:
    ObjectHeirarchy(String name);
    ~ObjectHeirarchy();

   

    // Inherited via AUIScreen
    virtual void drawUI() override;

    void onCreateInspectorScreen();

private:
    String m_name;
};

