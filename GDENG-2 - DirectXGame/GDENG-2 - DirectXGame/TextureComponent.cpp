#include "TextureComponent.h"

#include "GraphicsEngine.h"

TextureComponent::TextureComponent(std::string name, AGameObject* owner) : AComponent(name, ComponentType::Tex, owner)
{
	std::wstring path = L"Assets\\Textures\\wood.jpg";
	filepath = "Assets/Textures/wood.jpg";
	texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(path.c_str());
}

TextureComponent::~TextureComponent()
{
}

void TextureComponent::perform(float deltaTime)
{
}

TexturePtr TextureComponent::getTexture()
{
	return this->texture;
}

void TextureComponent::setPathAndLoad(std::string path)
{
}

std::string TextureComponent::getPath()
{
	return "hehe";
}

void TextureComponent::updateTexture(TexturePtr newtex)
{
	this->texture = newtex;
	this->getOwner()->updateTexture(newtex);
}
