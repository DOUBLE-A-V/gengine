#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <shader_s.h>
#include <Vector2.h>
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
		~Texture() {
			uint textures[1];
			textures[0] = texture;
			glDeleteTextures(1, textures);
		}
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
		~Character() {
			uint textures[1];
			textures[0] = texture;
			glDeleteTextures(1, textures);
		}
	};
	class Font {
	public:
		vector<Character*> chars;
		Character* getChar(int code);
		~Font() {
			for (Character* ch : chars) {
				delete ch;
			}
			int count = 0;
			for (Font* font : fonts) {
				if (font == this) {
					fonts.erase(fonts.begin() + count);
					break;
				}
				count++;
			}
		}
		string name;
	};
	class Vector3 {
	public:
		float x;
		float y;
		float z;
	};

	class Sprite {
	public:
		Vector2 position;
		float rotation = 0;
		void resize(int newWidth, int newHeight);
		Vector2 getSize();
		Render::Texture* texture = NULL;
		Sprite(const char* path) { this->texture = Render::loadTexture(path); };
		Sprite() { this->position = Vector2(0, 0); this->texture = NULL;};
		void checkChanges();
		~Sprite() {
			int count = 0;
			for (Sprite* sprite : sprites) {
				if (sprite == this) {
					sprites.erase(sprites.begin() + count);
					break;
				}
				count++;
			}
			delete texture;
		}
		void setTexture(Texture* texture);
	private:
		Vector2 size;
		Vector2 oldPosition;
		float oldRotation = 0;
	};
	class Text {
	public:
		Vector2 position;
		vector<uint> charsTextures;
		string text = "";
		Font* font;
		float size = 32;
		float charDistance = 8;
		float lineDistance = 8;
		float rotation = 0;

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

		vector<float> calcVerticesForChar(int num, int line);

		Text(string text_);
		void updateVertices();
		~Text() {
			int count = 0;
			for (Text* text : texts) {
				if (text == this) {
					texts.erase(texts.begin() + count);
					break;
				}
				count++;
			}
			delete font;
		}
	private:
		float oldRotation = 0;
		Vector2 oldPosition = Vector2(0, 0);
		string oldText = "";
		Font* oldFont;
		float oldSize = 1;
		Vector2 charPosChange;
		Vector2 linePosChange;
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