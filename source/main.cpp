#include <iostream>

#include <Gengine.h>
#include <GengineDevelopment.hpp>

using namespace std;
using namespace GengineDevelopment;

int main() {
	Window::init("fuck niggers 2", 800, 600);
	Render::loadFont("D:/font/font.davf", "default");
	Object image = createObject("some image", ObjectType::Sprite);
	
	Sprite sprite = image->getModifier<Sprite>("Sprite");
	sprite->setTexture(loadTexture("D:/daun.png"));


	Render::Text* text = Render::createText("abcdefg\nabc\ndef", 100, 100);
	text->size = 3;
	text->rotation = 0;
	Window::mainloop();
	return 0;
}