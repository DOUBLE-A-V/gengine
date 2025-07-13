#pragma once
#include <render.h>
class Window
{
public:
	static GLFWwindow* window;
	static int init(const char* title, int width, int height);
	static void mainloop();
	static void terminate();
	static void processInput(GLFWwindow* window);
};