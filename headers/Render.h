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

		Texture(uint id, vector<float> vertices, int width, int height, size_t verticesSize);
		void resize(int newWidth, int newHeight);
		void repos(int newx, int newy);
		void setFilter(int filter);
	};
	class Vector3 {
	public:
		float x;
		float y;
		float z;
	};
	class Vector2 {
	public:
		float x;
		float y;
		Vector2();
		Vector2(int x, int y);
		Vector2(float x, float y);
	};

	class Sprite {
	public:
		Vector2 position;
		void resize(int newWidth, int newHeight);
		Vector2 getSize();
		Render::Texture* texture;
		Sprite(const char* path) { this->texture = Render::loadTexture(path); };
		void checkChanges();
	private:
		Vector2 size;
		Vector2 oldPosition;
	};

	static uint texture;
	static float vertices[];
	static uint indices[];
	static uint VBO;
	static uint VAO;
	static uint EBO;
	static vector<Sprite*> sprites;

	static int windowWidth;
	static int windowHeight;

	static int init(GLFWwindow* renderWindow);
	static void terminate();
	static void renderFrame();

	static Render::Texture* loadTexture(const char* path);
private:
	static GLFWwindow* window;
	static Shader* ourShader;
};