#include <iostream>

#include <Gengine.h>

using namespace std;
using GE = Gengine;

int main() {
	Window::init("fuck niggers 2", 800, 600);
	Render::loadFont("D:/font/font.davf", "default");
	GE::Object* image = GE::createObject("some image", GE::ObjectType::Sprite);
	
	GE::Sprite* sprite = image->getModifier<GE::Sprite>("Sprite");

	Render::Text* text = Render::createText("abcdefg\nabc\ndef", 100, 100);
	text->size = 3;
	text->rotation = 0;
	Window::mainloop();
	return 0;
}