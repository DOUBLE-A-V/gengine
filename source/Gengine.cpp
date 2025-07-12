#include <Gengine.h>

Sprite* Gengine::createSprite(const char* texturePath) {
	Sprite* sprite = new Sprite(texturePath);
	sprite->position = Vector2(0, 0);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	return sprite;
}
Texture* Gengine::loadTexture(const char* path) {
	return Render::loadTexture(path);
}