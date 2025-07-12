#pragma once
#include <Render.h>
#include <Window.h>

using Vector2 = Render::Vector2;
using Sprite = Render::Sprite;
using Texture = Render::Texture;

class Gengine
{
public:
	static Sprite* createSprite(const char* texturePath);
	static Texture* loadTexture(const char* path);
};