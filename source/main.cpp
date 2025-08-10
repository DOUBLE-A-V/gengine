#include <iostream>

#include <GengineDevelopment.hpp>

using namespace std;
using namespace GengineDevelopment;

void start() {
	cout << "hello world from Gengine" << endl;
}
void testFixedUpdate() {
	if (getMouseButton(MOUSE_BUTTON_LEFT)) {
		mainTree->findFirstChild("just sprite")->rotation += 1;
	}
}
int main() {
	if (gengineInit("fuck this window", 800, 600) != 1) {
		return -1;
	}
	Object image = createSpriteObject("just sprite", "D:/daun.png");
	Object image2 = createSpriteObject("just another sprite", "D:/daun2.png");
	image2->position.x = 200;
	image2->setParent(image);
	setStartFunc(start);
	setFixedUpdateFunc(testFixedUpdate);


	startMainloop();
	return 0;
}