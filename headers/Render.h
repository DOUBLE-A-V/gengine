#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <shader_s.h>
#include <Vector3.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		float rotation;

		Texture(uint id, vector<float> vertices, int width, int height, size_t verticesSize);
		void resize(int newWidth, int newHeight);
		void repos(int newx, int newy);
		void setFilter(int filter);
		void updateVertices();
	};
	class Character {
	public:
		uint texture;
		uint width, height;
		uint charCode;
		Character(uint textureID, uint iwidth, uint iheight, uint icharCode) {
			this->texture = textureID;
			this->width = iwidth;
			this->height = iheight;
			this->charCode = icharCode;
		};
	};
	class Font {
	public:
		vector<Character*> chars;
		Character* getChar(int code);
		string name;
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

		operator string();
	};

	class Sprite {
	public:
		Vector2 position;
		float rotation = 0;
		void resize(int newWidth, int newHeight);
		Vector2 getSize();
		Render::Texture* texture;
		Sprite(const char* path) { this->texture = Render::loadTexture(path); };
		void checkChanges();
	private:
		Vector2 size;
		Vector2 oldPosition;
		float oldRotation;
	};
	class Text {
	public:
		Vector2 position;
		vector<uint> charsTextures;
		string text = "";
		Font* font;
		float size = 32;
		float charDistance = 18;
		vector<float> vertices = {
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
		};

		float alpha, red, green, blue;

		void setFont(string fontName);
		void setFont(Font* font);

		void checkChanges();

		vector<float> calcVerticesForChar(int num);

		Text(string text_);
		void updateVertices();
	private:
		Vector2 oldPosition = Vector2(0, 0);
		string oldText = "";
		Font* oldFont;
		float oldSize = 1;
		Vector2 charPosChange;
	};

	static uint texture;
	static float vertices[];
	static uint indices[];
	static uint VBO;
	static uint VAO;
	static uint EBO;
	static vector<Sprite*> sprites;
	static vector<Font*> fonts;
	static vector<Text*> texts;

	static int windowWidth;
	static int windowHeight;

	static int init(GLFWwindow* renderWindow);
	static void terminate();
	static void renderFrame();

	static void updateAllVertices();
	static Render::Font* loadFont(string path, string loadAsName);
	static float fps;

	static Render::Texture* loadTexture(const char* path);
	static uint createTextureFromData(unsigned char* data, uint width, uint height);

	static Text* createText(string text, int posx, int posy);
private:
	static GLFWwindow* window;
	static Shader* ourShader;
};