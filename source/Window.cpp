#include "Window.h"

GLFWwindow* Window::window;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void(*Window::startFunc)() = NULL;
void(*Window::updateFunc)(float) = NULL;
void(*Window::updateAllPoses)() = NULL;

chrono::system_clock::time_point Window::prevFrameTime = chrono::system_clock::now();

float Window::fps = 0;
double Window::deltaTime = 0;

bool Window::mainloopWorking = false;

int Window::init(const char* title, int width, int height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	Window::window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window::window);
	glfwSetFramebufferSizeCallback(Window::window, framebufferSizeCallback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Render::windowWidth = width;
	Render::windowHeight = height;

	Render::init(window);

	return 0;
}
void Window::terminate() {
	glfwSetWindowShouldClose(Window::window, true);
	Render::terminate();
	glfwTerminate();
}
void Window::mainloop() {
	mainloopWorking = true;
	if (startFunc) {
		startFunc();
	}
	while (mainloopWorking)
	{
		deltaTime = (chrono::system_clock::now() - prevFrameTime).count() / 10000000.0;
		prevFrameTime = chrono::system_clock::now();
		fps = 1.0 / deltaTime;
		if (updateFunc) {
			updateFunc(deltaTime);
		}
		updateAllPoses();
		if (glfwWindowShouldClose(window)) {
			break;
		}
		Render::renderFrame();
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Render::windowWidth = width;
	Render::windowHeight = height;
	Render::updateAllVertices();
}