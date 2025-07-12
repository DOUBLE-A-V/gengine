/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader_s.h>
*/

#include <iostream>

#include <Gengine.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;
using GE = Gengine;

int main() {
	Window::init("fuck niggers 2", 800, 600);
	Sprite* sprite = GE::createSprite("D:/daun.png");
	sprite->resize(400, 300);
	Window::mainloop();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}