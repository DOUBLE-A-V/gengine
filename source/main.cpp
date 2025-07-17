/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader_s.h>
*/

#include <iostream>

#include <Gengine.h>

using namespace std;
using GE = Gengine;

int main() {
	Window::init("fuck niggers 2", 800, 600);
	Sprite* sprite = GE::createSprite("D:/daun.png");
	sprite->resize(300, 300);
	sprite->rotation = 45;
	sprite->texture->alpha = 0;
	Render::loadFont("D:/font/font.davf", "default");
	Render::Text* text = Render::createText("fuck you", 0, 0);
	text->size = 1;
	Window::mainloop();
	return 0;
}