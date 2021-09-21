#pragma once
#include "AComponent.h"
#include "Prerequisites.h"
class Texture;

class TextureComponent :
    public AComponent
{

public:
	TextureComponent(std::string name, AGameObject* owner);
	~TextureComponent();

	void perform(float deltaTime) override;
	
	TexturePtr getTexture();
	void setPathAndLoad(std::string path);
	
	std::string getPath();
	void updateTexture(TexturePtr newtex);

private:
	std::string filepath;
	TexturePtr texture;
};

