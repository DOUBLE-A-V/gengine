#include "Window.h"

GLFWwindow* Window::window;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

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
	while (!glfwWindowShouldClose(Window::window))
	{
		Render::renderFrame();
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Render::windowWidth = width;
	Render::windowHeight = height;
}