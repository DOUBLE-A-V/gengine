#pragma once
#include <Render.h>
class Window
{
public:
	static float fps;
	static double deltaTime;

	static GLFWwindow* window;
	static int init(const char* title, int width, int height);
	static void mainloop();
	static void terminate();
	static void (*startFunc)();

	static void (*updateFunc)(float);

	static void(*updateAllPoses)();
private:
	static chrono::system_clock::time_point prevFrameTime;
};