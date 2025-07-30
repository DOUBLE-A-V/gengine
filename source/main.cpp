#include <iostream>

#include <Gengine.h>
#include <GengineDevelopment.hpp>

using namespace std;
using namespace GengineDevelopment;

int main() {
	if (gengineInit("fuck this window", 800, 600) != 1) {
		return -1;
	}
	cout << "initialized" << endl;
	Object image = createObject("some image", ObjectType::Sprite);
	
	Sprite sprite = image->getModifier<Sprite>("Sprite");
	sprite->setTexture(loadTexture("D:/daun.png"));

	for (int i = 5; i--; i > 0) {
		createObject("some object " + to_string(i), ObjectType::Object)->setParent(image);
	}
	createObject("fucking object", ObjectType::Object);

	Text text = createText("abcdefg\nabc\ndef");
	text->position.x = 400;
	text->size = 3;
	text->rotation = 0;
	cout << visualizeObjectTree(mainTree) << endl;

	startMainloop();
	return 0;
}