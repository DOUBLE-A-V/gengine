#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <shader_s.h>
#include <Vector3.h>
#include <iostream>
#include <vector>

using uint = unsigned int;
using namespace std;
class Render
{
public:
	class Texture
	{
	public:
		uint texture;
		vector<float> vertices;
		int width;
		int height;
		int posx;
		int posy;
		size_t verticesSize;

		float calcPosx;
		float calcPosy;
		float calcWidth;
		float calcHeight;

		float alpha;
		float red;
		float green;
		float blue;

		Texture(uint id, vector<float> vertices, int width, int height, int posx, int posy, size_t verticesSize);
		void resize(int newWidth, int newHeight);
		void setFilter(int filter);
	};

	static uint texture;
	static float vertices[];
	static uint indices[];
	static uint VBO;
	static uint VAO;
	static uint EBO;
	static vector<Texture*> textures;

	static int windowWidth;
	static int windowHeight;

	static int init(GLFWwindow* renderWindow);
	static void terminate();
	static void renderFrame();

	static Render::Texture* loadTexture(const char* path, int posx, int posy);
private:
	static GLFWwindow* window;
	static Shader* ourShader;
};