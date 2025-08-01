#include <iostream>

#include <GengineDevelopment.hpp>

using namespace std;
using namespace GengineDevelopment;

void start() {
	cout << "hello world from Gengine" << endl;
}
void testFixedUpdate() {
	if (getMouseButton(MOUSE_BUTTON_LEFT)) {
		mainTree->findFirstChild("just sprite")->getSpriteModifier()->rotation += 1;
	}

	cout << getFPS() << endl;
}
int main() {
	if (gengineInit("fuck this window", 800, 600) != 1) {
		return -1;
	}
	Object image = createSpriteObject("just sprite", "D:/daun.png");
	setStartFunc(start);
	setFixedUpdateFunc(testFixedUpdate);

	startMainloop();
	return 0;
}