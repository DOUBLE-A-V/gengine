#include <iostream>

#include <GengineDevelopment.hpp>

using namespace std;
using namespace GengineDevelopment;
void start() {
	cout << "hello world from Gengine" << endl;
}
void testUpdate(float delta) {
	if (getMouseButton(MOUSE_BUTTON_LEFT)) {
		mainTree->rotation += 20 * delta;
	}
	cout << mainTree->findFirstChild("just sprite")->getModifier<Collision>("Collision")->havePoint(getMousePos()) << " | " << (string)(getMousePos()) << endl;;
}
int main() {
	if (gengineInit("fuck this window", 800, 600) != 1) {
		return -1;
	}
	Object image = createSpriteObject("just sprite", "D:/daun.png");
	image->addModifier("Collision");
	image->getModifier<Collision>("Collision")->rect = Vector2(image->getSpriteModifier()->texture->width, image->getSpriteModifier()->texture->height);
	image->rotation = 45;
	setStartFunc(start);
	setUpdateFunc(testUpdate);

	startMainloop();
	return 0;
}