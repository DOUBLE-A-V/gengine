#include "Render.h"

uint Render::VBO, Render::VAO, Render::EBO;
GLFWwindow* Render::window;

Shader* Render::ourShader;
uint Render::texture;

float Render::vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
uint Render::indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

vector<Render::Sprite*> Render::sprites;
vector<Render::Font*> Render::fonts;
vector<Render::Text*> Render::texts;

int Render::windowWidth, Render::windowHeight;

float lastTime = 0.0f;
float Render::fps = 0;

const double pi = acos(-1);
const double radian = pi / 180.0;

int Render::init(GLFWwindow* renderWindow) {
	Render::window = renderWindow;

	glGenVertexArrays(1, &Render::VAO);
	glGenBuffers(1, &Render::VBO);
	glGenBuffers(1, &Render::EBO);

	glBindVertexArray(Render::VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Render::indices), Render::indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD); // this is default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Render::ourShader = new Shader("vertexShader.vs", "fragmentShader.fs");

	glfwSwapInterval(1);

	return 0;
}

void Render::terminate() {
	for (Render::Sprite* sprite : Render::sprites) {
		delete sprite->texture;
		delete sprite;
	}
	delete Render::ourShader;
	
	glDeleteVertexArrays(1, &Render::VAO);
	glDeleteBuffers(1, &Render::VBO);
	glDeleteBuffers(1, &Render::EBO);
	glfwTerminate();
}
void Render::renderFrame() {
	glClearColor(0, 0.5f, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Render::ourShader->use();
	for (Sprite* sprite : Render::sprites) {
		sprite->checkChanges();

		Render::ourShader->setFloat("alpha", sprite->texture->alpha);
		Render::ourShader->setFloat("red", sprite->texture->red);
		Render::ourShader->setFloat("green", sprite->texture->green);
		Render::ourShader->setFloat("blue", sprite->texture->blue);

		glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);
		glBufferData(GL_ARRAY_BUFFER, sprite->texture->verticesSize, sprite->texture->vertices.data(), GL_STATIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, sprite->texture->texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	for (Text* text : Render::texts) {
		text->checkChanges();
		int count = 0;
		for (uint texture : text->charsTextures) {
			Render::ourShader->setFloat("alpha", text->alpha);
			Render::ourShader->setFloat("red", text->red);
			Render::ourShader->setFloat("green", text->green);
			Render::ourShader->setFloat("blue", text->blue);

			glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);
			glBufferData(GL_ARRAY_BUFFER, 128, text->calcVerticesForChar(count).data(), GL_STATIC_DRAW);

			glBindTexture(GL_TEXTURE_2D, texture);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			count++;
		}
	}
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	fps = (1 / deltaTime);

	glfwSwapBuffers(Render::window);
	glfwPollEvents();
}
Render::Texture* Render::loadTexture(const char* path) {
	uint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	float calcPosx = (float)0 / windowWidth / 2; // posx instead of zero
	float calcPosy = (float)0 / windowHeight / 2;
	float calcWidth = ((float)width / 2) / (windowWidth / 2);
	float calcHeight = ((float)height / 2) / (windowHeight / 2);

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	Texture* textureObject = new Texture(texture, vector<float>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0])), width, height, sizeof(vertices));
	return textureObject;
}

uint Render::createTextureFromData(unsigned char* data, uint width, uint height) {
	uint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
Render::Vector2::operator string() {
	return "Vector2(" + to_string(x) + ", " + to_string(y) + ")";
}
void Render::Texture::updateVertices() {
	vertices[0] = calcWidth + calcPosx;
	vertices[1] = calcHeight + calcPosy;
	vertices[8] = calcWidth + calcPosx;
	vertices[9] = -calcHeight + calcPosy;
	vertices[16] = -calcWidth + calcPosx;
	vertices[17] = -calcHeight + calcPosy;
	vertices[24] = -calcWidth + calcPosx;
	vertices[25] = calcHeight + calcPosy;

	Vector2 center = Vector2(this->posx, this->posy);

	center.x = center.x * windowWidth;
	center.y = center.y * windowHeight;

	float width = this->width;
	float height = this->height;

	float s = sin(-rotation * radian);
	float c = cos(-rotation * radian);

	vertices[0] = (center.x + ((width / 2) * c) - ((height / 2) * s)) / windowWidth;
	vertices[1] = (center.y + ((width / 2) * s) + ((height / 2) * c)) / windowHeight;

	vertices[8] = (center.x + ((width / 2) * c) + ((height / 2) * s)) / windowWidth;
	vertices[9] = (center.y + ((width / 2) * s) - ((height / 2) * c)) / windowHeight;

	vertices[16] = (center.x - ((width / 2) * c) + ((height / 2) * s)) / windowWidth;
	vertices[17] = (center.y - ((width / 2) * s) - ((height / 2) * c)) / windowHeight;

	vertices[24] = (center.x - ((width / 2) * c) - ((height / 2) * s)) / windowWidth;
	vertices[25] = (center.y - ((width / 2) * s) + ((height / 2) * c)) / windowHeight;

	//delete& tmp;
	//delete& texPos;
}

void Render::updateAllVertices() {
	for (Sprite* sprite : Render::sprites) {
		sprite->texture->updateVertices();
	}
}

Render::Texture::Texture(uint id, vector<float> vertices, int width, int height, size_t verticesSize) {
	this->texture = id;
	this->vertices = vertices;
	this->width = width;
	this->height = height;
	this->posx = posx;
	this->posy = posy;
	this->verticesSize = verticesSize;

	this->calcPosx = (float)posx / (windowWidth / 2);
	this->calcPosy = (float)posy / (windowHeight / 2);
	this->calcWidth = ((float)width / 2) / (windowWidth / 2);
	this->calcHeight = ((float)height / 2) / (windowHeight / 2);
	this->alpha = 1;
	this->red = 1;
	this->green = 1;
	this->blue = 1;
}
void Render::Texture::setFilter(int filter) {
	glBindTexture(GL_TEXTURE_2D, this->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}
void Render::Texture::resize(int newWidth, int newHeight) {

	this->width = newWidth;
	this->height = newHeight;

	this->calcWidth = ((float)newWidth / 2) / (windowWidth / 2);
	this->calcHeight = ((float)newHeight / 2) / (windowHeight / 2);


	this->updateVertices();
}

void Render::Texture::repos(int newx, int newy) {
	this->posx = newx;
	this->posy = newy;

	this->calcPosx = (float)newx / (windowWidth / 2);
	this->calcPosy = (float)newy / (windowHeight / 2);

	this->updateVertices();
}

Render::Vector2::Vector2(int x, int y) {
	this->x = (float)x;
	this->y = (float)y;
}
Render::Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

void Render::Sprite::resize(int newWidth, int newHeight) {
	if (newWidth != this->size.x || newHeight != this->size.y) {
		this->texture->resize(newWidth, newHeight);
	}
	this->size.x = newWidth;
	this->size.y = newHeight;
}
Render::Vector2 Render::Sprite::getSize() {
	return this->size;
}
void Render::Sprite::checkChanges() {
	if (this->position.x != this->oldPosition.x || this->position.y != this->oldPosition.y) {
		this->texture->repos(this->position.x, this->position.y);
		this->oldPosition.x = this->position.x;
		this->oldPosition.y = this->position.y;
	}

	if (this->rotation != this->oldRotation) {
		texture->rotation = this->rotation;
		texture->updateVertices();

		oldRotation = rotation;
	}
}
Render::Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}

Render::Character* Render::Font::getChar(int code) {
	for (Character* ch : this->chars) {
		if (ch->charCode == code) {
			return ch;
		}
	}
}

Render::Font* Render::loadFont(string path, string loadAsName) {
	Font* font = new Font();
	font->name = loadAsName;
	ifstream f(path, ios::binary);
	char byte;
	vector<unsigned char> data;
	while (f.get(byte)) {
		data.push_back(byte);
	}
	f.close();

	if (data[0] == 68 && data[1] == 65 && data[2] == 86 && data[3] == 70) {
		unsigned int count = 0;
		int readingCharCode = 0;
		bool readingWidth = false, readingHeight = false, readingImageData = false;
		unsigned int count2 = 0;
		uint width = 0, height = 0;
		unsigned int charCodeByte1 = 0;
		unsigned int charCodeByte2 = 0;
		Character* tmpChar;
		vector<unsigned char> imageData;
		for (unsigned char byte : data) {
			if (count > 3) {
				if (count2 == 0 && readingImageData) {
					readingImageData = false;
					string strByte2 = to_string((uint)charCodeByte2);
					if (strByte2.size() == 1) {
						strByte2 = "0" + strByte2;
					}
					tmpChar = new Character(createTextureFromData(imageData.data(), width, height), width, height, stoi(to_string((uint)charCodeByte1) + strByte2));
					font->chars.push_back(tmpChar);
					imageData.clear();
				}
				if (count2 != 0) {
					imageData.push_back(byte);
					count2--;
				}
				if (!readingImageData) {
					if (readingCharCode == 1) {
						readingCharCode = 2;
						charCodeByte2 = byte;
					}
					if (readingCharCode == 2) {
						readingCharCode = 0;
						readingWidth = true;
					}
					else if (readingWidth) {
						width = byte;
						readingWidth = false;
						readingHeight = true;
					}
					else if (readingHeight) {
						height = byte;
						readingHeight = false;
						readingImageData = true;
						count2 = width * height * 4;
					}
				}
				if (readingCharCode == 0 && !readingWidth && !readingHeight && !readingImageData) {
					readingCharCode = 1;
					if (count != data.size() - 1) {
					}
					charCodeByte1 = byte;
				}
			}
			count++;
		}
		string strByte2 = to_string((uint)charCodeByte2);
		if (strByte2.size() == 1) {
			strByte2 = "0" + strByte2;
		}
		tmpChar = new Character(createTextureFromData(imageData.data(), width, height), width, height, stoi(to_string((uint)charCodeByte1) + strByte2));
		font->chars.push_back(tmpChar);
		fonts.push_back(font);
		return font;
	}
	else {
		cout << "it is not a DAVF file: " << path << endl;
	}
	return 0;
}

Render::Text::Text(string text_) {
	this->text = text_;
	this->oldText = text;
	this->size = 1;
	this->oldSize = 1;

	this->alpha = 1;
	this->red = 1;
	this->green = 1;
	this->blue = 1;

	Font* font = fonts[0];

	this->oldFont = font;
	this->font = font;

	for (char ch : text) {
		bool found = false;
		for (Character* fontChar : font->chars) {
			if (fontChar->charCode == ch) {
				this->charsTextures.push_back(fontChar->texture);
				found = true;
				break;
			}
		}
		if (!found) {
			this->charsTextures.push_back(font->getChar(0)->texture);
		}
	}
}

void Render::Text::checkChanges() {
	if (position.x != oldPosition.x || position.y != oldPosition.y || text != oldText || size != oldSize || oldRotation != rotation) {
		if (text != oldText) {
			this->oldText = text;
			charsTextures.clear();
			for (char ch : text) {
				bool found = false;
				for (Character* fontChar : font->chars) {
					if (fontChar->charCode == ch) {
						this->charsTextures.push_back(fontChar->texture);
						found = true;
						break;
					}
				}
				if (!found) {
					this->charsTextures.push_back(font->getChar(0)->texture);
				}
			}
		}
		this->oldPosition.x = this->position.x;
		this->oldPosition.y = this->position.y;

		this->oldSize = this->size;

		this->oldRotation = rotation;

		this->updateVertices();
	}
}
void Render::Text::updateVertices() {
	float calcWidth = ((float)32 * size / 2) / (windowWidth / 2);
	float calcHeight = ((float)32 * size / 2) / (windowHeight / 2);
	float calcPosx = (float)position.x / windowWidth / 2;
	float calcPosy = (float)position.y / windowHeight / 2;

	vertices[0] = calcWidth + calcPosx;
	vertices[1] = calcHeight + calcPosy;
	vertices[8] = calcWidth + calcPosx;
	vertices[9] = -calcHeight + calcPosy;
	vertices[16] = -calcWidth + calcPosx;
	vertices[17] = -calcHeight + calcPosy;
	vertices[24] = -calcWidth + calcPosx;
	vertices[25] = calcHeight + calcPosy;

	Vector2 center = this->position;

	float width = 32 * size / 2;
	float height = 32 * size / 2;

	float s = sin(-rotation * radian);
	float c = cos(-rotation * radian);

	vertices[0] = (center.x + ((width / 2) * c) - ((height / 2) * s)) / windowWidth;
	vertices[1] = (center.y + ((width / 2) * s) + ((height / 2) * c)) / windowHeight;

	vertices[8] = (center.x + ((width / 2) * c) + ((height / 2) * s)) / windowWidth;
	vertices[9] = (center.y + ((width / 2) * s) - ((height / 2) * c)) / windowHeight;

	vertices[16] = (center.x - ((width / 2) * c) + ((height / 2) * s)) / windowWidth;
	vertices[17] = (center.y - ((width / 2) * s) - ((height / 2) * c)) / windowHeight;

	vertices[24] = (center.x - ((width / 2) * c) - ((height / 2) * s)) / windowWidth;
	vertices[25] = (center.y - ((width / 2) * s) + ((height / 2) * c)) / windowHeight;

	charPosChange.x = (charDistance * size) / (windowHeight / 2) * c;
	charPosChange.y = (charDistance * size) / (windowHeight / 2) * s;
}

vector<float> Render::Text::calcVerticesForChar(int num) {
	vector<float> calced;
	for (float el : vertices) {
		calced.push_back(el);
	}
	Vector2 posChange = Vector2(charPosChange.x * num, charPosChange.y * num);

	calced[0] += posChange.x;
	calced[1] += posChange.y;
	calced[8] += posChange.x;
	calced[9] += posChange.y;
	calced[16] += posChange.x;
	calced[17] += posChange.y;
	calced[24] += posChange.x;
	calced[25] += posChange.y;

	return calced;
}

Render::Text* Render::createText(string text, int posx, int posy) {
	Text* textObj = new Text(text);
	textObj->position.x = posx;
	textObj->position.y = posy;
	textObj->updateVertices();

	texts.push_back(textObj);

	return textObj;
}