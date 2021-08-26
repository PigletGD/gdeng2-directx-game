#pragma once
#include "AUIScreen.h"
class InspectorScreen :
    public AUIScreen
{
public:
    InspectorScreen(String name);
    ~InspectorScreen();



    // Inherited via AUIScreen
    virtual void drawUI() override;

private:
    String m_name;

    friend class ObjectHeirarchy;
};

